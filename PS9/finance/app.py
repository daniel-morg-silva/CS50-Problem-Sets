import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, jsonify
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_id = session["user_id"]

    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]

    holdings = db.execute(
        "SELECT stock, stock_symbol, num_shares FROM holdings WHERE user_id = ?", user_id)
    sum = 0

    for stock in holdings:
        stock["stock_value"] = float(lookup(stock['stock_symbol'])["price"])
        stock["num_shares"] = int(stock["num_shares"])
        stock["total_value"] = usd(stock["stock_value"] * stock["num_shares"])
        sum += stock["stock_value"] * stock["num_shares"]

    return render_template("index.html", cash=cash, holdings=holdings, sum=sum)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        number_shares = request.form.get("shares")

        # if stock not found
        if stock == None:
            return apology("Stock Not Found", 400)
        # if stock not a number
        try:
            number_shares = float(number_shares)
        except:
            return apology("Number of shares must be an number", 400)
        # if stock lower that 1
        if number_shares < 1 or number_shares % 1 != 0:
            return apology("Number of shares must be an integer greater than 0", 400)

        user_id = session["user_id"]

        user = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        if len(user) != 1:
            return apology("Internal Server Problem", 500)

        if stock["price"] * number_shares > user[0]["cash"]:
            return apology("You do not have enough money to buy the stock(s)", 400)

        # substract the money from the users account
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ? ",
                   stock["price"] * number_shares, user_id)
        # add the transaction into the purchases table
        db.execute("INSERT INTO purchases (user_id, stock_symbol, stock, price, num_shares, date, type) VALUES ((SELECT id FROM users WHERE id = ?), ?, ?, ?, ?, ?, ?)",
                   user_id, stock['symbol'], stock["name"], stock["price"], number_shares, datetime.now(), "buy")

        row = db.execute("SELECT stock FROM holdings WHERE stock_symbol = ?", stock["symbol"])
        if not row:
            db.execute("INSERT INTO holdings (user_id, stock_symbol, stock, num_shares) VALUES ((SELECT id FROM users WHERE id = ?), ?, ?, ?)",
                       user_id, stock["symbol"], stock["name"], number_shares)
        else:
            db.execute("UPDATE holdings SET num_shares = num_shares + ? WHERE user_id = (SELECT id FROM users WHERE id = ?) AND stock = ?",
                       number_shares, user_id, stock["name"])

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT type, stock_symbol, price, num_shares, date FROM purchases")

    return render_template("history.html", transactions=transactions)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
    if request.method == "POST":
        stock = lookup(request.form.get("symbol"))

        if stock == None:
            return apology("Could not find the Stock")
        else:
            return render_template("quoted.html", stock=stock)

    else:
        return render_template("quote.html")


@app.route(f"/quote/<stock_symbol>")
@login_required
def quote_by_get(stock_symbol):
    """Get stock by GET"""
    stock = lookup(stock_symbol)

    return jsonify(stock)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")

        if not username:
            return apology("Must provide username", 400)

        if not password:
            return apology("Must provide password", 400)

        if not request.form.get("confirmation"):
            return apology("Must repeat the password", 400)
        if password != request.form.get("confirmation"):
            return apology("Passwords don't match", 400)

        if db.execute("SELECT username FROM users WHERE username = ?", username):
            return apology("User already exists", 400)

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                   username, generate_password_hash(password))

        session["user_id"] = db.execute("SELECT id FROM users WHERE username= ?", username)
        return redirect("/login")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        stock_symbol = request.form.get("symbol")
        num_shares_sell = int(request.form.get("shares"))

        num_shares_have = int(db.execute(
            "SELECT num_shares FROM holdings WHERE stock_symbol = ?", stock_symbol)[0]['num_shares'])

        if num_shares_sell > num_shares_have or num_shares_have == None:
            return apology("You don't have enough shares")

        stock = lookup(stock_symbol)
        user_id = session["user_id"]

        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?",
                   stock["price"] * num_shares_sell, user_id)
        db.execute("Update holdings SET num_shares = ? - ? WHERE stock_symbol = ?",
                   num_shares_have, num_shares_sell, stock["symbol"])
        db.execute("INSERT INTO purchases (user_id, stock_symbol, stock, price, num_shares, date, type) VALUES ((SELECT id FROM users WHERE id = ?), ?, ?, ?, ?, ?, ?)",
                   user_id, stock['symbol'], stock['name'], stock['price'], num_shares_sell, datetime.now(), "sell")

        return redirect("/")

    else:
        stocks_owned = db.execute(
            "SELECT stock_symbol FROM holdings WHERE user_id = ?", session["user_id"])

        return render_template("sell.html", stocks=stocks_owned)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change Password"""
    if request.method == "POST":
        current_password = request.form.get("current_password")
        new_password = request.form.get("new_password")

        user_id = session["user_id"]

        hash = db.execute("SELECT hash FROM users WHERE id = ?", user_id)[0]["hash"]

        if not check_password_hash(hash, current_password):
            return apology("Incorrect Password")

        db.execute("UPDATE users SET hash = ? WHERE id = ?",
                   generate_password_hash(new_password), user_id)

        return redirect("/")

    else:
        return render_template("change_password.html")
