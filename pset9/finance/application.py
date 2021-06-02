import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Save all useful information in variables
    userId = session["user_id"]
    cash = db.execute("SELECT cash FROM users WHERE id = ?", userId)
    balance = cash[0]["cash"]
    rows = db.execute("SELECT * FROM accounts WHERE user_id = ?", userId)

    # Iterate through each account and update all new data in SQL Database
    for i in range(len(rows)):
        symbol = rows[i]["symbol"]
        stock = lookup(symbol)
        price = float(stock['price'])
        shares = db.execute("SELECT shares FROM accounts WHERE user_id = ? AND symbol = ?", userId, symbol)
        totalValue = shares[0]["shares"] * price
        db.execute("UPDATE accounts SET price = ?, total = ? WHERE symbol = ? AND user_id = ?", price, totalValue, symbol, userId)

    # Retrieve new data from accounts
    updatedRows = db.execute("SELECT * FROM accounts WHERE user_id = ?", userId)

    # Calculate total sum of asset value
    assetValue = db.execute("SELECT SUM(total) FROM accounts WHERE user_id = ?", userId)

    # Avoid null error in case Asset Value is null (no stocks owned)
    if assetValue[0]['SUM(total)'] is None:
        total = balance
    else:
        total = assetValue[0]['SUM(total)'] + balance

    return render_template("index.html", stocks=updatedRows, balance=balance, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        # User forgets to input shares
        if not request.form.get("shares"):
            return apology("must enter number of shares", 400)

        # User forgets to input symbol
        if not request.form.get("symbol"):
            return apology("must enter symbol", 400)

        # Store symbol and share values in variable
        symbol = request.form.get("symbol")

        shares = request.form.get("shares")
        sharesValid = "True"

        for i in range(len(shares)):
            if not (ord(shares[i]) > 47 and ord(shares[i]) < 58):
                sharesValid = "False"

        if sharesValid == "False":
            return apology("shares must be an integer value", 400)

        shares = int(request.form.get("shares"))

        # User inputs invalid number of shares
        if shares < 1:
            return apology("invalid number of shares", 400)

        # Lookup does not exist
        if lookup(symbol) is None:
            return apology("invalid symbol", 400)

        # Store all useful data
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        userId = session["user_id"]
        stock = lookup(symbol)
        price = float(stock['price'])
        symbol = stock['symbol']
        name = stock['name']
        totalBalance = rows[0]["cash"]
        totalPurchase = price * shares

        # Retrieve current time
        now = datetime.now()
        time = now.strftime("%d/%m/%Y %H:%M:%S")

        # Check if user has enough funds
        if totalPurchase > totalBalance:
            return apology("not enough funds", 400)

        # Calculate user's new balance
        totalBalance = totalBalance - totalPurchase

        # Insert new transaction into SQL Table to appear in History tab
        db.execute("INSERT INTO transactions (user_id, symbol, price, shares, time, type) VALUES(?, ?, ?, ?, ?, ?)",
                   userId, symbol, price, shares, time, 'buy')

        rows = db.execute("SELECT * FROM accounts WHERE symbol = ? AND user_id = ?", symbol, userId)

        # If stock already exists, update new value by adding the amount of stocks bought
        if len(rows) > 0:
            prevShares = db.execute("SELECT shares FROM accounts WHERE symbol = ? AND user_id = ?", symbol, userId)
            shares = shares + prevShares[0]["shares"]
            totalValue = (price * shares)
            db.execute("UPDATE accounts SET shares = ?, price = ?, total = ? WHERE symbol = ? AND user_id = ?",
                       shares, price, totalValue, symbol, userId)
        # Else create a new row in accounts
        else:
            db.execute("INSERT INTO accounts (user_id, symbol, name, shares, price, total) VALUES(?, ?, ?, ?, ?, ?)",
                       userId, symbol, name, shares, price, totalPurchase)

        # Update user's cash amount
        db.execute("UPDATE users SET cash = ? WHERE id = ?", totalBalance, userId)

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Select user's transactions history and pass into history.html as transactions
    userId = session["user_id"]
    rows = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY id DESC", userId)

    return render_template("history.html", transactions=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "GET":
        return render_template("quote.html")

    else:
        # Get all relevant stock data and pass into quoted.html

        if not request.form.get("symbol"):
            return apology("must enter symbol", 400)

        symbol = request.form.get("symbol")

        if lookup(symbol) is None:
            return apology("Invalid symbol", 400)

        stock = lookup(symbol)
        name = stock['name']
        price = stock['price']
        symbol = stock['symbol']

        return render_template("quoted.html", symbol=symbol, name=name, price=price)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # If form was submitted, following code is executed
    if request.method == "POST":
        # Check to see if username is not empty
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Check to see if password is not empty
        if not request.form.get("password"):
            return apology("must provide password", 400)

        if len(request.form.get("password")) < 5:
            return apology("password must be over 5 characters long", 400)

        password = request.form.get("password")
        capitalExists = "False"

        for i in range(len(password)):
            if ord(password[i]) > 64 and ord(password[i]) < 91:
                capitalExists = "True"

        if capitalExists == "False":
            return apology("password must include capital letter", 400)

        # Check to see if confirmation is not empty and confirmation is equal to the password
        if not request.form.get("confirmation") or request.form.get("confirmation") != request.form.get("password"):
            return apology("passwords do not match", 400)

        # Check to see if username exists in database and store in rows
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # If a row is found, display error as username was found and already exists
        if len(rows) != 0:
            return apology("username already exists", 400)

        # Hash password and store in new variable
        hashedpassword = generate_password_hash(request.form.get("password"))

        # Insert new hash and password into database
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", request.form.get("username"), hashedpassword)

        # Redirect to login to login with new username and password
        return redirect("/login")

    # If initially redirected with no form, load the register.html form
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        # User forgets to input shares
        if not request.form.get("shares"):
            return apology("must enter number of shares", 400)

        # User forgets to input symbol
        if not request.form.get("symbol"):
            return apology("must enter symbol", 400)

        # Store info in variables
        symbol = request.form.get("symbol")

        shares = request.form.get("shares")
        sharesValid = "True"

        # Iterates through each character in share to ensure all characters are numbers
        for i in range(len(shares)):
            if not (ord(shares[i]) > 47 and ord(shares[i]) < 58):
                sharesValid = "False"

        # If one of the characters was not a number, input was not an integer
        if sharesValid == "False":
            return apology("shares must be an integer value", 400)

        # Successfuly convert shares to integer after checks
        shares = int(request.form.get("shares"))

        # Check to see if shares is greater than 1
        if shares < 1:
            return apology("invalid number of shares", 400)

        # Symbol Exists operates as a Boolean that will check if user owns stock
        userId = session["user_id"]
        symbolExists = "False"

        rows = db.execute("SELECT symbol FROM accounts WHERE user_id = ?", userId)

        # Iterate through all of user's accounts to check if symbol exists
        for i in range(len(rows)):
            if rows[i]["symbol"] == symbol:
                symbolExists = "True"

        # If symbolExists was never updated to True, user does not own stock and error message is returned
        if symbolExists == "False":
            return apology("you do not own this stock", 400)

        rows = db.execute("SELECT shares FROM accounts WHERE user_id = ? AND symbol = ?", userId, symbol)

        # Check if user has enough shares
        if shares > rows[0]["shares"]:
            return apology("not enough shares", 400)

        # Retrieve current date and time
        now = datetime.now()
        time = now.strftime("%d/%m/%Y %H:%M:%S")

        # Get relevant stock data
        stock = lookup(symbol)
        price = stock['price']
        revenue = price * shares

        # Insert data into transactions table to be later displayed in history
        db.execute("INSERT INTO transactions (user_id, symbol, price, shares, time, type) VALUES(?, ?, ?, ?, ?, ?)",
                   userId, symbol, price, shares, time, 'sell')

        # Retrieve user's cash and add revenue
        rows = db.execute("SELECT cash FROM users WHERE id = ?", userId)
        cash = rows[0]['cash']
        cash = cash + revenue
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, userId)

        # Update account with less shares, if shares = 0 then delete
        rows = db.execute("SELECT shares FROM accounts WHERE user_id = ? AND symbol = ?", userId, symbol)
        shares = rows[0]['shares'] - shares

        # If user no longer has shares, delete symbol from accounts
        if shares == 0:
            db.execute("DELETE FROM accounts WHERE user_id = ? AND symbol = ?", userId, symbol)
        # Else, update the new amount of shares in the SQL account table
        else:
            db.execute("UPDATE accounts SET shares = ? WHERE user_id = ? AND symbol = ?", shares, userId, symbol)

        return redirect("/")

    else:
        # Pass account information to show what stocks to display in dropdown menu
        userId = session["user_id"]
        rows = db.execute("SELECT * FROM accounts WHERE user_id = ?", userId)
        return render_template("sell.html", accounts=rows)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
