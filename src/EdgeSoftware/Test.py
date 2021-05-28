"""
This file contains the unit tests for the Aggregator class in Aggregator.py
"""

import random
import unittest
from Aggregator import Aggregator


def generate_random_array(length):
    l = []
    for e in range(length):
        l.append(random.randint(0, 100))

    return l


def get_array_max(arr):
    max_value = 0
    for e in arr:
        if e > max_value:
            max_value = e
    return max_value


def get_array_min(arr):
    min_value = 2 ** 32
    for e in arr:
        if e < min_value:
            min_value = e
    return min_value


def get_array_avg(arr):
    sum_value = 0
    for e in arr:
        sum_value += e
    return sum_value / len(arr)


class TestAggregator(unittest.TestCase):
    aggregator = Aggregator()

    def test_temperature(self):
        values = generate_random_array(10)
        for e in values:
            self.aggregator.add_temperature(e)

        self.assertEqual(self.aggregator.get_temp_max(), get_array_max(values))
        self.assertEqual(self.aggregator.get_temp_avg(), get_array_avg(values))
        self.assertEqual(self.aggregator.get_temp_min(), get_array_min(values))

    def test_humidity(self):
        values = generate_random_array(10)
        for e in values:
            self.aggregator.add_hum(e)

        self.assertEqual(self.aggregator.get_hum_max(), get_array_max(values))
        self.assertEqual(self.aggregator.get_hum_avg(), get_array_avg(values))
        self.assertEqual(self.aggregator.get_hum_min(), get_array_min(values))

    def test_soil(self):
        values = generate_random_array(10)
        for e in values:
            self.aggregator.add_soil(e)

        self.assertEqual(self.aggregator.get_soil_max(), get_array_max(values))
        self.assertEqual(self.aggregator.get_soil_avg(), get_array_avg(values))
        self.assertEqual(self.aggregator.get_soil_min(), get_array_min(values))

    def test_hash(self):
        self.aggregator = Aggregator()
        last_hash = self.aggregator.hash_code()
        for e in range(10):
            self.aggregator.add_soil(random.randint(0, 100))
            self.aggregator.add_hum(random.randint(0, 100))
            self.aggregator.add_temperature(random.randint(0, 100))

            self.assertNotEqual(last_hash, self.aggregator.hash_code())
            last_hash = self.aggregator.hash_code()


if __name__ == '__main__':
    unittest.main()
