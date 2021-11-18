#include "my_strategy.hpp"
#include <fstream>
#include <iomanip>

using namespace std;

Data::Data(const std::string line):hour{line.substr(0,2)},
                                        minute{line.substr(3,2)},
                                        price{stod(line.substr(13, string::npos))}
{}

Bar::Bar(Data& data_obj): hour{data_obj.hour},
                          minute{data_obj.minute},
                          open_price{data_obj.price},
                          high_price{data_obj.price},
                          low_price{data_obj.price},
                          close_price{data_obj.price},
                          fast_avg{0.},
                          slow_avg{0.}
{}


vector<Data> get_data(const string&  input_filename) {

    vector<Data> data_list;
    ifstream input_file(input_filename);
    string line;
    if (!input_file.is_open())
        return data_list;
    while (getline(input_file,line)) {
        Data data_obj(line);
        data_list.push_back(data_obj);
    }
    input_file.close();
    return data_list;
}

vector<Bar> generate_bars(const vector<Data>& data_list) {

    string last_minute = "";
    vector<Bar> bar_list;

    for(int i = 0; i < data_list.size(); ++i) {
        auto data_obj = data_list[i];

        if(data_obj.minute != last_minute) {
            last_minute = data_obj.minute;
            Bar bar_obj(data_obj);
            bar_list.push_back(bar_obj);
        }
        else {
            auto& curr_bar = bar_list.back();
            curr_bar.high_price = max(curr_bar.high_price, data_obj.price);
            curr_bar.low_price = min(curr_bar.low_price, data_obj.price);
            curr_bar.close_price = data_obj.price;

        }
    }
    return bar_list;
}

void get_moving_average(vector<Bar>& bar_list, const int fast_len, const int slow_len){

    double f_run_sum = 0, s_run_sum = 0;

    for(int i = 0; i < bar_list.size(); i++) {
        auto& bar_obj = bar_list[i];
        f_run_sum += bar_obj.close_price;
        s_run_sum += bar_obj.close_price;

        if(i >= fast_len-1) {
            if(i == fast_len - 1)
                bar_obj.fast_avg = f_run_sum/fast_len;
            else {
                f_run_sum -= bar_list[i-fast_len].close_price;
                bar_obj.fast_avg = f_run_sum/fast_len;
            }
        }

        if(i >= slow_len-1) {
            if(i == slow_len -1)
                bar_obj.slow_avg = s_run_sum/slow_len;
            else {
                s_run_sum -= bar_list[i-slow_len].close_price;
                bar_obj.slow_avg = s_run_sum/slow_len;
            }
        }
    }
}

bool write_signal(const string&  output_filename, const vector<Bar>& bar_list,
                  const int fast_len, const int slow_len ) {

    ofstream output_file(output_filename);

    if(!output_file.is_open())
       return false;

    output_file<<"TIME, OPEN, HIGH, LOW, CLOSE, FASTMA, SLOWMA, SIGNAL\n";
    int count = 1;

    for(const auto& bar_obj : bar_list) {
        output_file<<bar_obj.hour<<":"<<bar_obj.minute<<","
                   <<fixed<<setprecision(2)<<bar_obj.open_price<<","
                   <<bar_obj.high_price<<","<<bar_obj.low_price<<","
                   <<bar_obj.close_price<<","<<setprecision(4)<<bar_obj.fast_avg<<","
                   <<bar_obj.slow_avg<<",";

        if(count >= slow_len){
            if(bar_obj.fast_avg > bar_obj.slow_avg)
                output_file<<"BUY";
            else if(bar_obj.fast_avg < bar_obj.slow_avg)
                output_file<<"SELL";
        }

        output_file<<"\n";
        ++count;
    }

    output_file.close();

    return true;

}
