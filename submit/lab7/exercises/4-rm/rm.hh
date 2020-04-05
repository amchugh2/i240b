#ifndef OUT_HH_
#define OUT_HH_

#include <algorithm>
#include <vector>


template <typename Iterator>
void
outContainer(Iterator begin, Iterator end) 
{
  std::cout << "[ ";
  auto print = [](const int& i) { std::cout << i << " " ; };
  std::for_each(begin, end, print);
  std::cout << "]" << std::endl;
}

/** create a vector which is identical to [begin, end) but with first
 *  occurrence of arg removed.  Output vector elements within [ ],
 *  separated by spaces.
 */
template <typename Iterator>
void
loopFn(Iterator begin, Iterator end, int arg)
{
  std::vector<int> vec;
  bool is_seen = false;
  //@TODO
  for(auto p = begin; p != end; ++p){
	if(*p != arg){
		vec.push_back(*p);
	}
	if(*p == arg){
		if(is_seen == false){
			is_seen = true;
		}
		vec.push_back(*p);
	}
  }

  outContainer(vec.begin(), vec.end());
}

/** Exactly like loopFn() but no loops should be used */
template <typename Iterator>
void noLoopFn(Iterator begin, Iterator end, int arg)
{
  //@TODO
  auto x = std::find(begin, end, arg);
  std::vector<int> vec;
  std::copy(begin, end, std::back_inserter(vec));

  if (x != end){
  	std::copy(++x, end, std::back_inserter(vec));
  }
  outContainer(vec.begin(), vec.end());  
}

#endif //ifndef OUT_HH_