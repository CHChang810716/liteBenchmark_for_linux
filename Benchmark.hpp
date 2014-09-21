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
        std::ostream& program_std_out;
        std::ostream& program_err_out;
public:
        Time_data userTime;
        Time_data systemTime;
        Time_data realTime;
        Benchmark()
        : program_std_out(std::cout)
        , program_err_out(std::cerr)
        {}
        Benchmark(std::string program_path)
        : program_std_out(std::cout)
        , program_err_out(std::cerr)
        {
                argv.push_back("time");
                argv.push_back("-f=timestart=\n%e\n%U\n%S\n");
                argv.push_back(program_path);
        }
        Benchmark(std::string program_path, std::ostream& out, std::ostream& err)
        : program_std_out(out)
        , program_err_out(err)
        {
                argv.push_back("time");
                argv.push_back("-f=timestart=\n%e\n%U\n%S\n");
                argv.push_back(program_path);
        }
        void add_arg(std::string arg)
        {
                argv.push_back(arg);
        }
        /*void set_stdout(std::string str)
        {

        }*/
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
