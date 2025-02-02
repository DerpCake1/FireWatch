import requests
import pandas as pd
import time
import os

channel_id = '2826828'
read_api_key = 'YL0E6VXZF2HSF0AK'

url = f'https://api.thingspeak.com/channels/{channel_id}/feeds.json'

params = {
    'api_key': read_api_key,
    'results': 1000
}


def fetch_data():
    if os.path.exists("thingspeak_data.csv"):
        os.remove('thingspeak_data.csv')
    response = requests.get(url, params=params)

    if response.status_code == 200:
        json_data = response.json()

        df = pd.DataFrame(json_data['feeds'])

        df = df.tail(20)
        df.rename(columns={'field1': 'Humidity', 'field2': 'Temperature', 'field3': 'Wind Speed'}, inplace=True)
        df.drop('entry_id', axis=1, inplace=True)

        df.to_csv('thingspeak_data.csv', mode='a', header=False, index=False)
        print("Data successfully saved to thingspeak_data.csv")

        return df
    else:
        print(f"Failed to fetch data. Status code: {response.status_code}")

def fire_risk(humidity, temperature, wind_speed):
    if humidity < 15 and wind_speed > 25 and temperature > 30:
        return 1
    else:
        return 0

while True:
    data = fetch_data()
    print(data)

    last_5_values = data.tail(5).drop(columns=['created_at'])

    last_5_values['Humidity'] = last_5_values['Humidity'].astype(float)
    last_5_values['Temperature'] = last_5_values['Temperature'].astype(float)
    last_5_values['Wind Speed'] = last_5_values['Wind Speed'].astype(float)

    column_sums = last_5_values.sum()

    avg_humidity = column_sums['Humidity'] / 5
    avg_temp = column_sums['Temperature'] / 5
    avg_wind_speed = column_sums['Wind Speed'] / 5

    result = "Yes" if fire_risk(avg_humidity, avg_temp, avg_wind_speed) else "No"
    print(f"Risk of Fire: {result}")
    time.sleep(10)