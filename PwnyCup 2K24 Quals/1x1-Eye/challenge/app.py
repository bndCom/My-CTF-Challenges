from flask import Flask, send_file

app = Flask(__name__)

@app.route("/spy.png", methods=["GET"])
def spy_image():
    return send_file("pixel.png", mimetype="image/jpeg")

@app.route("/", methods=["GET"])
def get_flag():
    return "<h1>nOt foUNd</h1> <! shellmates{eM41L$_HAvE_3ye$_b3_caREFul!!} >"

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=80)
