import requests
import json
import datetime
import sys

API_URL = "https://ssd.jpl.nasa.gov/api/horizons.api"

BODIES = {
    "Sun":     {"id": "10",  "mass": 1.0,         "radius": 0.5,  "color": [1.0, 1.0, 0.0], "emissive": True},
    "Mercury": {"id": "199", "mass": 1.6601e-7,   "radius": 0.05, "color": [0.5, 0.5, 0.5], "emissive": False},
    "Venus":   {"id": "299", "mass": 2.4478e-6,   "radius": 0.1,  "color": [0.9, 0.9, 0.9], "emissive": False},
    "Earth":   {"id": "399", "mass": 3.0035e-6,   "radius": 0.1,  "color": [0.0, 0.0, 1.0], "emissive": False},
    "Mars":    {"id": "499", "mass": 3.2271e-7,   "radius": 0.08, "color": [1.0, 0.5, 0.0], "emissive": False},
    "Jupiter": {"id": "599", "mass": 9.5479e-4,   "radius": 0.3,  "color": [0.8, 0.6, 0.4], "emissive": False},
    "Saturn":  {"id": "699", "mass": 2.8588e-4,   "radius": 0.25, "color": [0.9, 0.8, 0.5], "emissive": False},
    "Uranus":  {"id": "799", "mass": 4.3662e-5,   "radius": 0.15, "color": [0.0, 1.0, 1.0], "emissive": False},
    "Neptune": {"id": "899", "mass": 5.1514e-5,   "radius": 0.15, "color": [0.0, 0.0, 0.5], "emissive": False}
}

def fetch_vector(body_id):
    today = datetime.datetime.now().strftime("%Y-%m-%d")
    tomorrow = (datetime.datetime.now() + datetime.timedelta(days=1)).strftime("%Y-%m-%d")
    params = {
        "format": "json", "COMMAND": f"'{body_id}'", "OBJ_DATA": "'NO'", "MAKE_EPHEM": "'YES'",
        "EPHEM_TYPE": "'VECTORS'", "CENTER": "'@0'", "START_TIME": f"'{today}'",
        "STOP_TIME": f"'{tomorrow}'", "STEP_SIZE": "'1d'", "VEC_TABLE": "'2'",
        "REF_SYSTEM": "'J2000'", "OUT_UNITS": "'AU-D'", "CSV_FORMAT": "'YES'"
    }
    try:
        response = requests.get(API_URL, params=params)
        data = response.json()
        result_lines = data["result"].split('\n')
        start_idx = next(i for i, line in enumerate(result_lines) if "$$SOE" in line) + 1
        csv_data = result_lines[start_idx].split(',')

        # Original: X, Y, Z  -> [2], [3], [4]
        # New:      X, Z, Y  -> [2], [4], [3]
        # We swap Y and Z so the orbital plane lies on the engine's X/Z "floor"
        return {
            "pos": [float(csv_data[2]), float(csv_data[4]), float(csv_data[3])],
            "vel": [float(csv_data[5]), float(csv_data[7]), float(csv_data[6])]
        }
    except Exception:
        return None

def main():
    scene_data = []
    for name, info in BODIES.items():
        vectors = fetch_vector(info["id"])
        if vectors:
            scene_data.append({
                "name": name, "mass": info["mass"], "radius": info["radius"],
                "color": info["color"], "is_emissive": info["emissive"],
                "position": vectors["pos"], "velocity": vectors["vel"]
            })
    with open("solar_system_data.json", "w") as f:
        json.dump(scene_data, f, indent=4)

if __name__ == "__main__":
    main()