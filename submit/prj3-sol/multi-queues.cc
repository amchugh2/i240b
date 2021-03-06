#include <iostream>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <memory>

#include "job.hh"
#include "command.hh"
#include "command-stream.hh"

//using namespace Command::Cmd;

template <typename Job> 
class heap {
	private:
		typedef bool (*comparator)(const Job&, const Job&);
		comparator cmp;
		void swp(std::vector<Job>& heap, int a, int b){
			const Job temp = heap[a];
			heap[a] = heap[b];
			heap[b] = temp;
		}

		void sift_down(std::vector<Job>& heap, int i){
			if(i*2+1 > heap.size()){
					return;
			}
			int ind = i;

			if(i*2+1 > heap.size()-1) { return;}
			if(i*2+2 > heap.size()-1){
				if(cmp(heap[i*2+1], heap[ind])){
					ind = i*2+1;
				}
			}
			else {
				if(cmp(heap[i*2+1], heap[ind]) == false){
					ind = i*2+1;
				}
				if(cmp(heap[i*2+1], heap[ind]) == false){
					ind = i*2+2;
				}
			}
			if(ind != i){
				swp(heap, ind, i);
				sift_down(heap, ind);
			}
		}
	public:
		heap(comparator cmp) : cmp(cmp) {}
		void heapify(std::vector<Job>& heap) {
			for(int i = heap.size()/2; i > -1; i++){
				sift_down(heap, i);
			}
		}
		const Job pop(std::vector<Job>& heap){
			const Job ret = heap[0];
			heap[0] = heap.back();
			heap.pop_back();
			sift_down(heap,0);
			return ret;
		}
		void push(std::vector<Job>& heap, const Job& el){
			heap.push_back(el);
			int ind = heap.size() - 1;
			while(cmp(heap[ind], heap[(ind-1)/2])){
				swp(heap, ind, (ind-1)/2);
				ind = (ind-1)/2;
			}
		}
};
	void enter(heap<Job>& heap0, std::vector<Job>& vec, const Job& job){
		heap0.push(vec, job);
	}
	
	// need to output
	void leave(heap<Job>& heap1, std::vector<Job>& vec){
		// check if empty
		if(vec.size() == 0){
			return;
		}
		else {
		// pop from vector
			std::cout << heap1.pop(vec);
		 
		}
	}
	
	void move(heap<Job>& heap0, heap<Job>& heap1, std::vector<Job>& vec0, std::vector<Job>& vec1){
		// check if empty
		if(vec0.size() == 0){
			return;
		}
		else {
		// arrange q0 so we know we're getting rid of the right job
		const Job& highest_priority = heap0.pop(vec0);
		// push highest priority job onto q1 and then rearrange
		heap1.push(vec1, highest_priority);
		std::cout << highest_priority;
		}
	
	}

	bool cmp_gtr(const Job& a, const Job& b){
		if(a.priority0 == b.priority0){
			return a.id > b.id;
		}
		return a.priority0 > b.priority0;
	}

	bool cmp_less(const Job& a, const Job& b){
		if(a.priority1 == b.priority0){
			return a.id < b.id;
		}
		return a.priority1 < b.priority1;
	}


//std::unique_ptr<Command> Command::read(std::istream& in);

int main(int argc, char* argv[]){

	if(argc != 2){
		std::cerr << "usage: " << argv[0] << " FILE_PATH" << std::endl;
		std::exit(1);
	}
	// TODO: compare by lexographic if same priority

	// read input
	CommandStream commandIn(argv[1]); // create command stream
	
	// create the two heaps (should be inside class, not in main)
	std::vector<Job> q0;
	std::vector<Job> q1;

	heap<Job> heap0(cmp_gtr);
	heap<Job> heap1(cmp_less);
	
	// read input
	for(std::unique_ptr<Command> commandP = commandIn.next(); commandP != nullptr; commandP = commandIn.next()){
		Command cmd = *commandP;
		
		if(cmd.cmd == Command::Cmd::ENTER){
			EnterCommand* enterP = dynamic_cast<EnterCommand*>(commandP.get());
			Job job = enterP->job;
			enter(heap0, q0, job);
			std::cout << *commandP << std::endl;
		}
		else if(cmd.cmd == Command::Cmd::LEAVE){
			if(q1.size() != 0){
				std::cout << *commandP << " ";
				leave(heap1,q1);
				std::cout << std::endl;
			}
		}
		else if(cmd.cmd == Command::Cmd::MOVE){
			if(q0.size() != 0){
				std::cout << *commandP << " ";
				move(heap0, heap1, q0, q1);
				std::cout<< std::endl;
			}
		}
	else{
			std::cout << "Invalid command. Usage: 'COMMAND ID PRIORITY0 PRIORITY1'" << std::endl;
		}
	}	
	return 0;
}


