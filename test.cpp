#include <iostream>
#include <string>
#include <cmath>
#include "MyConfiguration.hpp"

using namespace std;

int main()
{
    /**
     * @mainpage
     * A sample run for 'MyConfiguration' header.
     * Reading a configuration file.
     */
    config my_config("config.cfg");
    my_config.print_content();

    uint64_t n_iteration;
    try
    {
        cout << "\n\nReading a string value: " << my_config.get_value_string("datafile");
        cout << "\n\nReading a numeric value: " << my_config.get_value("alpha", 0.9);

        cout << "\n\nReading a non-negative integer value: ";
        double signed_v = my_config.get_value("n_iteration", 50);
        if (signed_v < 0)
        {
            cout << "n_iteration: ";
            throw invalid_argument("The parameter cannot be negative!\n");
        }
        if (signed_v - floor(signed_v))
        {
            cout << "n_iteration: ";
            throw invalid_argument("The parameter must be an integer!\n");
        }
        n_iteration = (uint64_t)signed_v;
        cout << n_iteration << "\n";

        cout << "\n\nReading a missing numeric value, returning the default value:\n " << my_config.get_value("beta", 0.89);
        cout << "\n\nReading a missing string value: " << my_config.get_value_string("datafile2");

        cout << "\n\nThis value will not be read: " << my_config.get_value("theta", 0.8);
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << '\n';
        return -1;
    }
}
