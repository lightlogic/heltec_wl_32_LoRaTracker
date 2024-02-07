import csv
import random
from geopy.distance import geodesic
from geopy.point import Point

# Original location
original_location = Point(46.96369, 7.12487)

# Function to generate a random location within 5 km
def generate_random_location(base_location, max_distance_km):
    # Random bearing (direction) in degrees
    bearing = random.uniform(0, 360)
    # Random distance in kilometers
    distance = random.uniform(0, max_distance_km)
    # Calculate new location
    new_location = geodesic(kilometers=distance).destination(base_location, bearing)
    return new_location

# Generate 100 random locations
random_locations = [generate_random_location(original_location, 5) for _ in range(100)]

# Write locations to a CSV file, limiting significant figures to 7
csv_file_path = '/mnt/data/random_locations_limited.csv'
with open(csv_file_path, 'w', newline='') as file:
    writer = csv.writer(file)
    for location in random_locations:
        # Format latitude and longitude with 7 significant figures
        lat = f"{location.latitude:.7f}"
        lon = f"{location.longitude:.7f}"
        writer.writerow([f"{lat} {lon}"])

print("CSV file with random locations has been created.")
