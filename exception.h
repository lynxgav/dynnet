#ifndef EXCEPTION_H
#define EXCEPTION_H 

#include<sstream>
#include<iomanip>
#include<string>
#include<iostream>

template <class T>
std::string stringify(T x, int width=15, const char ch=' ')
 {
   std::ostringstream o;
   if (!(o << std::setw(width)<<std::setfill(ch)<<x))
     std::cerr<<"Bad coversion to string"<<std::endl;
   return o.str();
 }

#define ERROR(b, m)  if(b){std::cerr<<"ERROR: In file \""<< __FILE__<<"\", function \""<<  __PRETTY_FUNCTION__<<"\", line "<< __LINE__<<": "<<m<<"!"<<std::endl; exit(1);}
#define WARNING(b, m)  if(b){std::cerr<<"WARNING: In file \""<< __FILE__<<"\", function \""<<  __PRETTY_FUNCTION__<<"\", line "<< __LINE__<<": "<<m<<"!"<<std::endl; }

#endif /* EXCEPTION_H */
