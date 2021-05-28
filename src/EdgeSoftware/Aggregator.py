"""
This file contains the class Aggregator that performs the aggregation operations
"""


class Aggregator:
    temp_min = 0
    temp_max = 0
    temp_counter = 0
    temp_sum = 0

    hum_min = 0
    hum_max = 0
    hum_counter = 0
    hum_sum = 0

    soil_min = 0
    soil_max = 0
    soil_counter = 0
    soil_sum = 0

    def add_temperature(self, temp):

        if temp < self.temp_min or self.temp_counter == 0:
            self.temp_min = temp

        if temp > self.temp_max or self.temp_counter == 0:
            self.temp_max = temp

        self.temp_sum += temp
        self.temp_counter += 1

    def add_hum(self, hum):

        if hum < self.hum_min or self.hum_counter == 0:
            self.hum_min = hum

        if hum > self.hum_max or self.hum_counter == 0:
            self.hum_max = hum

        self.hum_sum += hum
        self.hum_counter += 1

    def add_soil(self, soil):

        if soil < self.soil_min or self.soil_counter == 0:
            self.soil_min = soil

        if soil > self.soil_max or self.soil_counter == 0:
            self.soil_max = soil

        self.soil_sum += soil
        self.soil_counter += 1

    def get_temp_avg(self):
        return self.temp_sum / self.temp_counter

    def get_hum_avg(self):
        return self.hum_sum / self.hum_counter

    def get_soil_avg(self):
        return self.soil_sum / self.soil_counter

    def get_temp_max(self):
        return self.temp_max

    def get_temp_min(self):
        return self.temp_min

    def get_hum_max(self):
        return self.hum_max

    def get_hum_min(self):
        return self.hum_min

    def get_soil_max(self):
        return self.soil_max

    def get_soil_min(self):
        return self.soil_min

    def are_there_some_data(self):
        return self.temp_counter > 0 and self.hum_counter > 0 and self.soil_counter > 0

    def hash_code(self):
        str_values = str(self.temp_sum) + str(self.temp_sum) \
                     + str(self.hum_sum) + str(self.hum_counter) \
                     + str(self.soil_sum) + str(self.soil_counter)
        return hash(str_values)
