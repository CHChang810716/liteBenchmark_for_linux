#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include "include/pstream.h"
#include <string>
#include <iostream>
#include <iomanip>
#include "Time_data.hpp"
#include <string>
#include <sstream>
#include <fstream>

using redi::pstreams;


template < class out_type = std::ostream, class err_type = std::ostream >
class Benchmark_kernal
{
protected:
    bool cmd_flag;
    pstreams::argv_type argv;
    out_type* program_std_out;
    err_type* program_err_out;

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
    void exec_program()
    {
        const pstreams::pmode streams =
        pstreams::pstdout|pstreams::pstderr;
        redi::pstream ps(argv[0], argv, streams);
        std::cout << "exec over\n";
        std::string buf;

        while (std::getline(ps.out(), buf))
        {
            //std::cout << "stdout:";
            *program_std_out << buf <<'\n';
        }
        ps.clear();
        std::cout << "stdout over\n";
        while (std::getline(ps.err(), buf))
        {
            if( !buf.compare("=timestart=") )
                    break;
            *program_err_out << buf <<'\n';
        }
        std::cout << "stderr over\n";
        std::getline(ps.err(), buf);
        realTime.push_back(std::stod (buf));
        std::getline(ps.err(), buf);
        userTime.push_back(std::stod (buf));
        std::getline(ps.err(), buf);
        systemTime.push_back(std::stod (buf));
    }

public:
    Time_data userTime;
    Time_data systemTime;
    Time_data realTime;

    Benchmark_kernal()
    :cmd_flag(false)
    {
        argv.push_back("time");
        argv.push_back("-f=timestart=\n%e\n%U\n%S\n");
    }

    /*Benchmark_kernal(std::string cmd)
    :cmd_flag(false)
    {
        argv.push_back("time");
        argv.push_back("-f=timestart=\n%e\n%U\n%S\n");
    }*/

    virtual void set_cmd(std::string cmd)
    {
        std::cout<<"kernal\n";
        if(cmd_flag)
        {
            //throw a exception here
            return ; 
        }
                
        std::vector<std::string> arg_set = split(cmd,' ');
        for(auto it = arg_set.begin(); it != arg_set.end(); it++)
        {
            argv.push_back(*it);
        }
        cmd_flag = true;
    }
    virtual void execute()
    {
        exec_program();
    }

    Benchmark_kernal operator+ (Benchmark_kernal& op)
    {
        Benchmark_kernal b;
        b.userTime = this -> userTime + op.userTime;
        b.systemTime = this -> systemTime + op.systemTime;
        b.realTime = this -> realTime + op.realTime;
        return b;
    }

    ~Benchmark_kernal()
    {
        program_std_out = NULL;
        program_err_out = NULL;
    }
};

template < class out_type = std::ostream, class err_type = std::ostream >
class Benchmark
:public Benchmark_kernal< out_type, err_type >
{
public:
    Benchmark()
    : Benchmark_kernal< out_type, err_type > ()
    {}

    Benchmark(std::string cmd)
    : Benchmark_kernal< out_type, err_type > (cmd)
    {
        this->set_cmd(cmd);
    }
    
    ~Benchmark()
    {}
};

template < class err_type >
class Benchmark< std::ofstream, err_type >
:public Benchmark_kernal< std::ofstream, err_type >
{
    std::string fname;
public:
    Benchmark()
    : Benchmark_kernal< std::ofstream, err_type > ()
    {
        this->program_std_out = new std::ofstream();
        this->program_err_out = &std::cerr;
    }

    Benchmark(std::string cmd)
    : Benchmark_kernal< std::ofstream, err_type > ()
    {
        this->program_std_out = new std::ofstream();
        this->program_err_out = &std::cerr;
        set_cmd(cmd);
    }

    void execute()
    {
        this->program_std_out -> open(fname);
        std::cout << "pos open\n";
        this->exec_program();
        std::cout << "pos exec\n";
        this->program_std_out -> close();

    }

    virtual void set_cmd(std::string cmd)
    {
        std::cout<<"fout\n";
        if(this->cmd_flag)
        {
            //throw a exception here
            return ; 
        }
                
        std::vector<std::string> arg_set = this->split(cmd,' ');
        for(auto it = arg_set.begin(); it < arg_set.end(); it++)
        {
            if( it->compare(">")==0 )
            {
                it ++;
                fname = *it;
                std::cout << "file name : " << fname << '\n';
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
                std::cout << *it << '\n';
                this->argv.push_back(*it);
            }
            std::cout << "line 204\n";
        }
        this->cmd_flag = true;
        std::cout << "line 206\n";
    }
    
    ~Benchmark()
    {
        delete this->program_std_out;
    }
};
#endif
