from flask import Flask, render_template, request, redirect, url_for
import requests

app = Flask(__name__)

# URL of the Firebase Realtime Database
firebase_url = 'https://jaljeevan-de36a-default-rtdb.asia-southeast1.firebasedatabase.app/'

# Function to fetch data from Firebase
def get_firebase_data():
    response = requests.get(firebase_url + 'FlowMeter.json')
    if response.status_code == 200:
        return response.json()
    else:
        return None

# Route to display the HTML form
@app.route('/')
def index():
    data = get_firebase_data()
    return render_template('index.html', data=data)

# Route to update DesiredLitres in Firebase
@app.route('/update', methods=['POST'])
def update_desired_litres():
    desired_litres = int(request.form['desired_litres'])
    response = requests.patch(firebase_url + 'FlowMeter.json', json={'DesiredLitres': desired_litres})
    if response.status_code == 200:
        return redirect(url_for('index'))
    else:
        return "Failed to update DesiredLitres."

if __name__ == '__main__':
    app.run(debug=True)
