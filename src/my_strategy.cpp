#include "my_strategy.hpp"


using namespace std;

int Run(std::string const& input_filename, std::string const& output_filename) {

    auto data_list = get_data(input_filename);
    if(data_list.size() == 0)
        return -1;

    auto bar_list = generate_bars(data_list);
    const int fast_len = 21, slow_len = 55;
    get_moving_average(bar_list, fast_len, slow_len);

    if(!write_signal(output_filename, bar_list, fast_len, slow_len))
        return -1;

    return 0;
}

int main(){
    return Run("./data/market_data.csv", "./data/signals.csv");
}