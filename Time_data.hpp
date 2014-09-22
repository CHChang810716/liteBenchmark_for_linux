#ifndef TIME_DATA_HPP
#define TIME_DATA_HPP 

#include <vector>
#include <cmath>
#include <initializer_list>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <functional>
#include <algorithm>    // std::for_each
#include <boost/accumulators/statistics/variance.hpp>
class Time_data
        : public std::vector<double>
{
public:
        Time_data(std::initializer_list<double> l) : std::vector<double>(l)
        {}
        Time_data()
        {}
        double get_sum()
        {
                double sum = 0;
                for( double d : *this )
                {
                        sum += d;
                }
                return sum;
        }
        double get_avg()
        {
                return get_sum() / this->size();
        }
        double get_sd()
        {
                using namespace std::placeholders;
                using namespace boost::accumulators;
                accumulator_set<double, stats<tag::variance> > acc;
                for_each(this->begin(), this->end(), std::bind<void>(std::ref(acc), _1));
                return std::sqrt(variance(acc));
        }
        Time_data append(Time_data& td)
        {
                Time_data res;
                for(auto v : *this)
                {
                        res.push_back(v);
                }
                for(auto v2 : td)
                        res.push_back(v2);
                return res;
        }
        Time_data operator+ (Time_data& td)
        {
                Time_data res;
                for( int i = 0; i < this->size(); i++ )
                {
                        res.push_back( (*this)[i] + td[i] );
                }
                return res;
        }
};
#endif