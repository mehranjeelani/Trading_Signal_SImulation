#pragma once
#include <string>
#include<vector>

typedef struct Data {
    std::string hour;
    std::string minute;
    double price;
    Data(const std::string line);

} Data;

typedef struct Bar {
    std::string hour;
    std::string minute;
    double open_price;
    double high_price;
    double low_price;
    double close_price;
    double fast_avg;
    double slow_avg;
    Bar();
    Bar(Data& data_obj);

} Bar;

int Run(std::string const& input_filename, std::string const& output_filename);

std::vector<Data> get_data(const std::string&  input_filename);

std::vector<Bar> generate_bars(const std::vector<Data>& data_list);

void get_moving_average(std::vector<Bar>& bar_list, const int fast_len,
                        const int slow_len);

bool write_signal(const std::string&  output_filename, const std::vector<Bar>& bar_list,
                  const int fast_len, const int slow_len );