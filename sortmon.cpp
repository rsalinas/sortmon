/*!
 * \brief sortmon - Monitor an output by sorting it. 
 * 
 * Written by Raul Salinas-Monteagudo <rausalinas@gmail.com>
 */

#include <string>
#include <list>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

using namespace std;

/*!
 * \brief Keeps the N highest elements from a pipelined input.
 * 
 */
class PipeSorter {
	public:
	PipeSorter(size_t sz) : sz_(sz) {
	}
	
	bool add(const std::string &s) {
		bool changed = false;
		auto sn = atoll(s.c_str());		
		decltype(l_.begin()) pos = l_.end();
		if (!l_.empty() && sn > atoll(l_.back().c_str())) {			
			for (auto it = l_.begin(), end=l_.end(); it != end; ++it) {
				if (sn > atoll(it->c_str())) {
					pos=it; 				
					break;
				}
			}
			changed = true;
			l_.insert(pos, s);
			if (l_.size() == sz_) {
				l_.pop_back();					
			}
		} else {
			if (l_.size() < sz_ - 1) {
				changed = true;
				l_.push_back(s);
			}
		}
		
		return changed;
	}
		
	friend ostream & operator<< (ostream & os , const PipeSorter & ps);

private:
	list<string> l_;
	const size_t sz_;
};

ostream & operator <<(ostream &os, const PipeSorter & ps) {		
		for (auto it = ps.l_.begin(), end = ps.l_.end() ; it != end; ++it) {			
			os << *it << '\n';
		}
		return os;
}

class SortMon {
public:	
	SortMon(const struct winsize &w) : pipeSorter_(w.ws_row-1), width_(w.ws_col) {
	}
	
	int run() {
		string l;
		time_t last_ = start_;
		bool changed = false;
		while (getline(cin, l)) {
			
			n++;
			if (l.size() > width_ ) {
				l.resize(width_);
			}
			if (pipeSorter_.add(l)) 
				changed = true;
			if (changed) {
				auto now = time(0);
				if (now - last_ >= 3) {
					last_=now;
					print(now);
				}	
				changed=false;		
			}
		}
		print(time(0));
		return 0;
	}
		
private:
	size_t n = 0;
	const int width_;
	const time_t start_= time(0);
	PipeSorter pipeSorter_;
		
	void print(const time_t &now) {
			cout << '\e' << "[H" << '\e' << "[2J"
				<< n << " lines. "<< " runtime: " << (now - start_)  << '\n'
				<< pipeSorter_
				<< std::flush;
	}
};

int main()
{	
	struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) < 0) {
		fprintf(stderr, "Error reading terminal size\n");
		return 1;
	}
	
	return (SortMon(w)).run();
}
