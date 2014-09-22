#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include "include/pstream.h"
#include <string>
#include <iostream>
#include <iomanip>
#include "Time_data.hpp"
#include <string>
#include <sstream>

using redi::pstreams;

class Benchmark
{

private:
        pstreams::argv_type argv;
        std::ostream* program_std_out;
        std::ostream* program_err_out;
        bool fout_flag;
        bool cmd_flag;
        
protected:
        void add_arg(std::string arg)
        {
                argv.push_back(arg);
        }

        std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
        {
            std::stringstream ss(s);
            std::string item;
            while (std::getline(ss, item, delim)) 
            {
                elems.push_back(item);
            }
            return elems;
        }


        std::vector<std::string> split(const std::string &s, char delim) 
        {
            std::vector<std::string> elems;
            split(s, delim, elems);
            return elems;
        }

public:
        Time_data userTime;
        Time_data systemTime;
        Time_data realTime;

        Benchmark()
        : fout_flag (false)
        , cmd_flag  (false)
        {
                program_std_out = &std::cout;
                program_err_out = &std::cerr;

                argv.push_back("time");
                argv.push_back("-f=timestart=\n%e\n%U\n%S\n");
        }

        Benchmark(std::string cmd)
        {
                argv.push_back("time");
                argv.push_back("-f=timestart=\n%e\n%U\n%S\n");
                set_cmd(cmd);

        }
        void set_cmd(std::string cmd)
        {
                if(cmd_flag)
                {
                        //throw a exception here
                        return ; 
                }
                        
                std::vector<std::string> arg_set = split(cmd,' ');
                for(auto it = arg_set.begin(); it != end(); it++)
                {
                        if( it->compare(">")==0 )
                        {
                                it++;
                                program_std_out = new std::ofstream (*it);
                                fout_flag = true;
                        }
                        else if( it->compare("<")==0 )
                        {
                                //input implementat
                        }
                        else if( it->compare("&>")==0 )
                        {
                                //merge output implementat
                        }
                        else
                        {
                                argv.push_back(*it);
                        }
                }
                cmd_flag = true;
        }

        
        void execute()
        {
                const pstreams::pmode streams =
                pstreams::pstdout|pstreams::pstderr;
                redi::pstream ps(argv[0], argv, streams);
                std::string buf;

                while (std::getline(ps.out(), buf))
                {
                        //std::cout << "stdout:";
                        program_std_out << buf <<'\n';
                }
                ps.clear();
                while (std::getline(ps.err(), buf))
                {
                        if( !buf.compare("=timestart=") )
                                break;
                        program_err_out << buf <<'\n';
                }

                std::getline(ps.err(), buf);
                realTime.push_back(std::stod (buf));
                std::getline(ps.err(), buf);
                userTime.push_back(std::stod (buf));
                std::getline(ps.err(), buf);
                systemTime.push_back(std::stod (buf));
        }

        void execute(int n)
        {
                for(int i = 0; i < n; i++)
                {
                        const pstreams::pmode streams =
                        pstreams::pstdout|pstreams::pstderr;
                        redi::pstream ps(argv[0], argv, streams);
                        std::string buf;
                        while (std::getline(ps.out(), buf))
                        {
                                //std::cout << "stdout:";
                                program_std_out << buf <<'\n';
                        }
                        ps.clear();
                        while (std::getline(ps.err(), buf))
                        {
                                if( !buf.compare("=timestart=") )
                                        break;
                                program_err_out << buf <<'\n';
                        }
                        
                        std::getline(ps.err(), buf);
                        realTime.push_back(std::stod (buf));
                        std::getline(ps.err(), buf);
                        userTime.push_back(std::stod (buf));
                        std::getline(ps.err(), buf);
                        systemTime.push_back(std::stod (buf));
                }
        }

        Benchmark operator+ (Benchmark& op)
        {
                Benchmark b;
                b.userTime = this -> userTime + op.userTime;
                b.systemTime = this -> systemTime + op.systemTime;
                b.realTime = this -> realTime + op.realTime;
                return b;
        }

};
#endif