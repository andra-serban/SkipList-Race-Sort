// Copyright  2019 Andra Serban <serban_andra96@yahoo.com>
#include <string.h>
#include <iostream>
#include <fstream>
#include <random>
#include <limits>
#include <algorithm>
#define HMAX 10
#define NMAX 550

struct Info {
	int id;
	int punctaj;
	int timp;
	int order;
	int order_ant;

	Info() {
		this->timp = 0;
		this->order = -1;
		this->punctaj = 0;
		this->order_ant = -1;
	}

	void set_id(int id) {
		this->id = id;
	}

	void set_order(int order) {
		this->order = order;
	}

	void set_old_points(int punctaj) {
		this->punctaj = punctaj;
	}

	void set_time(int timp) {
		this->timp = timp;
	}

	bool operator<(const Info& other) {
		 return (this->timp < other.timp || (this->timp == other.timp &&
		 	this->punctaj > other.punctaj) ||
		 	(this->timp == other.timp &&  this->punctaj == other.punctaj &&
		 	this->id < other.id));
	}

	bool operator>(const Info& other) {
		return (this->timp > other.timp);
	}

	bool operator==(const Info& other) {
		return (this->id == other.id);
	}

	bool operator!=(const int& other) {
			return this->timp != other;
	}

	void operator=(const int& other) {
		this->timp = other;
	}

	void operator=(const Info& other) {
		this->id = other.id;
		this->timp = other.timp;
		this->punctaj = other.punctaj;
	}

	friend std::ostream& operator << (std::ostream &out, const Info &other);
};

std::ostream& operator<< (std::ostream &out, const Info &other) {
	out << other.id << " " <<other.punctaj << " ";
    return out;
}

template <class T>
struct Node {
	T value;
	Node *next;
	Node *prev;
	Node *up;
	Node *down;

	Node() {
		this->next = NULL;
		this->prev = NULL;
		this->up = NULL;
		this->down = NULL;
	}
};

template <class Type1, class Type2>
class Skip_List {
 private:
	Node<Type1>* head;
	Node<Type1>* tail;
	int height = 1;

 public:
	Skip_List() {
		Node<Type1>* p1 = new Node<Type1>();
		Node<Type1>* p2 = new Node<Type1>();
		p2->value = std::numeric_limits<Type2>::max();
		p1->value  = std::numeric_limits<Type2>::min();
		p1->next = p2;
		p2->prev = p1;
		this->head = p1;
		this->tail = p2;
	}

	~Skip_List() {
		for(int i = 1; i <= height; ++i) {
			Node<Type1>* new_head = head->down;
			Node<Type1>* new_tail = tail->down;
			Node<Type1>* p = head;
			while(p != NULL) {
				head = head->next;
				if ( p != NULL) delete p;
				p = head;
			}
			head = new_head;
			tail = new_tail;
		}
	}

	Skip_List(const Skip_List& other) {
		this->head = other.head;
		this->tail = other.tail;
		this->height = other.height;
	}

	void operator=(const Skip_List& other) {
		this->head = other.head;
		this->tail = other.tail;
		this->height = other.height;
	}

	void create_new_level(){
		Node<Type1>* p1 = new Node<Type1>();
		Node<Type1>* p2 = new Node<Type1>();
		p1->value = std::numeric_limits<Type2>::min();
		p2->value = std::numeric_limits<Type2>::max();
		p1->next = p2;
		p2->prev = p1;
		p1->down = head;
		head->up = p1;
		p2->down = tail;
		tail->up = p2;
		this->head = p1;
		this->tail = p2;
		this->height++;
	}

	void print_skip_list() {
		Node<Type1>* p = head;
		while (p != NULL) {
			Node<Type1>* q = p;
			while (q != NULL) {
				std::cout << q->value << " ";
				q = q->next;
			}
			p = p->down;
			std::cout << std::endl;
		}
	}

	Node<Type1>* search_elem(Type1 elem) {
		Node<Type1>* p = head;
		while(true) {
			while (p->next->value < elem) {
				p = p->next;
			}
			if (p->down != NULL) {
				p = p->down;
			} else {
				break;
			}
		}
		return p;
	}

	Node<Type1>* find_elem(Type1 elem) {
		Node<Type1>* p = head;
		while (p->down != NULL) {
			p = p->down;
		}
		while(p->next != NULL) {
			if(p->next->value == elem) {
				return p;
			}
			p = p->next;
		}
		return tail;
	}

	int coin_flip() {
		std::random_device rd;
		std::uniform_int_distribution<int> dist(0, 1);
		return dist(rd);
	}

	Node<Type1>* find_left_up(Node<Type1>* p) {
		while(p->up == NULL) {
			p = p->prev;
		}
		return p->up;
	}

	void put(Type1 elem, Node<Type1>* p, Node<Type1>* q, int level) {
		Node<Type1>* newElem = new Node<Type1>();
		newElem->value = elem;
		if (level == 1) {
			p->next->prev = newElem;
			newElem->next = p->next;
			p->next = newElem;
			newElem->prev = p;
		} else if (level > 1) {
			newElem->down = p->next;
			p->next->up = newElem;
			q->next->prev = newElem;
			newElem->next = q->next;
			q->next = newElem;
			newElem->prev = q;
		}
	}

	void insert(Type1 elem) {
	Node<Type1>* p = search_elem(elem);
		int level = 1;
		put(elem, p, NULL, level);
		while (coin_flip()) {
			level++;
			if (level > height) {
				create_new_level();
			}
			Node<Type1>* q = find_left_up(p);
			put(elem, p, q, level);
			coin_flip();
			p = q;
		}
	}

	int empty_level() {
		if (head->next == tail) {
			return 1;
		}
		return 0;
	}

	void remove_level() {
		if(head->down != NULL && tail->down != NULL) {
			Node<Type1>* new_head = head->down;
			Node<Type1>* new_tail = tail->down;
			Node<Type1>* p = head;
			while(p != NULL) {
				head = head->next;
				if ( p != NULL) delete p;
				p = head;
			}
			head = new_head;
			tail = new_tail;
		}
		height--;
	}

	int get_pozition(Type1& elem) {
		Node<Type1>* p = head;
			int count = 0;
			while (p->down != NULL) {
				p = p->down;
			}
			while(p->next != NULL) {
				if (p->value != 0 ) count++;
				if(p->next->value == elem) break;
				p = p->next;
			}
			return count;
	}

	void remove_elem(Type1 elem) {
		Node<Type1>* p = find_elem(elem);
		if (p != tail) {
			p = p->next;
			while (p != NULL) {
				p->next->prev = p->prev;
				p->prev->next = p->next;
				Node<Type1>* q = p->up;
				delete p;
				p = q;
			}
			while (empty_level()){
				remove_level();
			}
		}
	}
};

void set_points(Skip_List<Info, int>& list_of_players, Info players[],
	int nr_players, int nr_of_participants
	) {
	for (int j = 1; j <= nr_players; j++) {
		if (players[j] != 0) {
			int id = players[j].id;
			Info aux = players[j];
			players[j] = players[id];
			players[id] = aux;
			int count = list_of_players.get_pozition(players[j]);
			if (count >  (nr_of_participants / 2)) {
				players[j].punctaj += -(nr_of_participants / 2)
				+ (nr_of_participants - (count));
			} else {
				players[j].punctaj += (nr_of_participants / 2) - (count - 1);
			}
		}
	}
}

bool comp(Info player1, Info player2) {
	if (player1.punctaj < player2.punctaj) {
		return 0;
	} else if (player1.punctaj == player2.punctaj && player1.id > player2.id) {
		return 0;
	} else {
		return 1;
	}
}

void print_clasament(Info clasament[], int nr_players, Info players[]) {
	for (int r  = 1; r <= nr_players; r++) {
			std::cout << clasament[r] <<" ";
			if(clasament[r].order_ant == -1) {
				std:: cout <<'0' <<std::endl;
				clasament[r].order = r;
				players[clasament[r].id].order = r;
				clasament[r].order_ant = 0;
			} else {
			std::cout << players[clasament[r].id].order - r << std::endl;
			players[clasament[r].id].order = r;
			}
		}
		std::cout <<std::endl;
}

int main() {
	FILE* in;
	in = fopen("races.in", "r");
	freopen("races.out", "w", stdout);
	int nr_players, nr_of_races, nr_prints;
	Skip_List<Info, int> list_of_players;
	struct Info players[NMAX];
	struct Info clasament[NMAX];
	char *info = new char[10];
	fscanf(in, "%d %d %d", &nr_players, &nr_of_races , &nr_prints);
	int prints_read = 0;
	int print_results = 0;
	for (int i = 1; i <= nr_of_races + 1; ++i) {
		int nr_of_participants = nr_players;
		for(int  j = 1; j <= nr_players; ++j){
			fscanf(in, "%s", info);
			if(strcmp(info, "print") != 0) {
				if (atoi(info) == 0) {
						nr_of_participants--;
					}
					if(i >= 2) {
						list_of_players.remove_elem(players[j]);
					}
					players[j].set_id(j);
					players[j].set_old_points(players[j].punctaj);
					players[j].set_time(atoi(info));
					list_of_players.insert(players[j]);

			} else {
				print_results = 1;
				prints_read++;
				if (prints_read == nr_prints) {
					break;
				}
				j--;
			}
		}
		for (int r = 1; r <= nr_players; r++) {
			clasament[r] = players[r];
		}
		std::sort(clasament + 1, clasament + nr_players + 1, comp);
		if(print_results) {
			print_clasament(clasament, nr_players, players);
		}
		print_results = 0;
		set_points(list_of_players, players, nr_players, nr_of_participants);
	}
	delete[] info;
	fclose(in);
}
