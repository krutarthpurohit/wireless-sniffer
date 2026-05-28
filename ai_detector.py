import pandas as pd
from sklearn.ensemble import IsolationForest
import time

print("Starting Edge AI RSSI Analyzer...");

while True:
        try:
            df = pd.read_csv("/home/dhruvpi/wireless-sniffer/rssi_logger.csv")
        
            if len(df) < 10:
                    print("Waiting for more data...")
                    time.sleep(2)
                    continue

            X = df[['rssi']]

            model = IsolationForest(
                contamination=0.1,
                random_state=42
            )

            model.fit(X)

            predictions = model.predict(X)

            df['anomaly'] = predictions

            latest = df.iloc[-1]

            print("-----------------------------")
            print(f"RSSI : {latest['rssi']} dBm")

            if latest['anomaly'] == -1:
                print("STATUS : ANOMALY DETECTED")
                print("Possible interference or instability")
            else:
                print("STATUS : NORMAL")

            time.sleep(2)

        except Exception as e:
            print(f"Error: {e}")
            time.sleep(2)