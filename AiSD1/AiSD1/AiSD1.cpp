#include <iostream>
#include <sstream>
#include <cstring>
using namespace std;



struct CharNode {
    char c;
    CharNode* next;
};

struct StackItem {
    CharNode* head;
    StackItem* next;
};

struct Stack {
    StackItem* top;

    Stack() : top(nullptr) {}

    ~Stack() {
        while (top) {
            StackItem* temp = top;
            top = top->next;
            delete_list(temp->head);
            delete temp;
        }
    }

    void push_empty() {
        StackItem* item = new StackItem;
        item->head = nullptr;
        item->next = top;
        top = item;
    }

    void push_copy() {
        if (!top) return;
        StackItem* item = new StackItem;
        item->head = copy_list(top->head);
        item->next = top;
        top = item;
    }

    void swap_top_two() {
        if (!top || !top->next) return;
        StackItem* first = top;
        StackItem* second = top->next;
        first->next = second->next;
        second->next = first;
        top = second;
    }

    void pop() {
        if (!top) return;
        StackItem* temp = top;
        top = top->next;
        delete_list(temp->head);
        delete temp;
    }

    void push_list(CharNode* list) {
        StackItem* item = new StackItem;
        item->head = list;
        item->next = top;
        top = item;
    }

    void print() {
        print_recursive(top, 0);
    }

    CharNode* detach_first_char() {
        if (!top || !top->head) return nullptr;
        CharNode* node = top->head;
        top->head = node->next;
        node->next = nullptr;
        return node;
    }

  /*  void append_list_to_top(CharNode* list) {
        if (!top) return;
        CharNode** p = &top->head;
        while (*p) p = &(*p)->next;
        *p = list;
    }*/

    void negate_number() {
        if (!top) return;
        CharNode* head = top->head;

        CharNode* prev = nullptr;
        CharNode* curr = head;
        while (curr && curr->next) {
            prev = curr;
            curr = curr->next;
        }

        if (curr && curr->c == '-') {
            if (prev)
                prev->next = nullptr;
            else
                top->head = nullptr;
            delete curr;
        }
        else {
            CharNode* node = new CharNode{ '-', nullptr };
            if (!head)
                top->head = node;
            else {
                CharNode* tail = head;
                while (tail->next) tail = tail->next;
                tail->next = node;
            }
        }
    }

    void absolute_value() {
        if (!top || !top->head) return;
        CharNode* p = top->head;
        CharNode* prev = nullptr;
        while (p->next) {
            prev = p;
            p = p->next;
        }
        if (p->c == '-') {
            if (prev)
                prev->next = nullptr;
            else
                top->head = nullptr;
            delete p;
        }
    }

    void pop_list_only() {
        if (!top) return;
        StackItem* temp = top;
        top = top->next;
        delete_list(temp->head);
        delete temp;
    }

    void pop_number_only() {
        if (!top) return;
        StackItem* temp = top;
        top = top->next;
        delete_list(temp->head);
        delete temp;
    }

    /*void execute_dollar() {
        CharNode* ch = detach_first_char();
        push_list(ch);
    }*/

    void execute_dollar() {
        CharNode* ch = detach_first_char();
        if (ch) {
            push_list(ch);
        }
    }

  /*  void execute_hash() {
        if (!top || !top->next) return;
        CharNode* A = copy_list(top->head);
        pop();
        CharNode* B = top->head;
        CharNode** p = &B;
        while (*p) p = &(*p)->next;
        *p = A;
    }*/

    void execute_hash() {
        if (!top || !top->next) return;
        CharNode* A = copy_list(top->head);
        pop();
        CharNode* B = top->head;
        CharNode** p = &B;
        while (*p) p = &(*p)->next;
        *p = A;
        top->head = B; // Upewnij się, że lista jest przypisana
    }


    void push_number_from_int(const int* n_ptr) {
        if (n_ptr) push_list(from_number(*n_ptr));
    }

   /* void execute_equal() {
        if (!top || !top->next) return;
        int A = to_number(top->head);
        int B = to_number(top->next->head);
        pop(); pop();
        push_list(new CharNode{ char((B == A) + '0'), nullptr });
    }*/

    void execute_equal() {
        if (!top || !top->next) return;
        int A = to_number(top->head);
        int B = to_number(top->next->head);
        pop();
        pop();
        CharNode* result = new CharNode{ char((B == A) + '0'), nullptr };
        push_list(result);
    }

    void execute_less() {
        if (!top || !top->next) return;
        int A = to_number(top->head);
        int B = to_number(top->next->head);
        pop(); pop();
        push_list(new CharNode{ char((B < A) + '0'), nullptr });
    }

    void execute_not() {
        if (!top) return;
        CharNode* h = top->head;
        bool is_false = (!h || (h->c == '0' && h->next == nullptr));
        pop();
        push_list(new CharNode{ char(is_false ? '1' : '0'), nullptr });
    }

    void execute_jump(int& ip) {
        if (!top || !top->next) return;
        int T = to_number(top->next->head);
        CharNode* W = top->head;
        bool jump = W && !(W->c == '0' && W->next == nullptr);
        pop(); pop();
        if (jump) {
            if (T == ip) return;
            ip = T;
        }
    }

    void execute_plus() {
        if (!top || !top->next) return;
        int A = to_number(top->head);
        StackItem* second = top->next;
        int B = to_number(second->head);
        pop(); pop();
        push_list(from_number(B + A)); // zachowana kolejność
    }

    void execute_bracket_close() {
        if (!top) return;
        int A = to_number(top->head);
        pop();
        char c = static_cast<char>(A & 0xFF);
        CharNode* node = new CharNode{ c, nullptr };
        push_list(node);
    }


private:
    CharNode* copy_list(CharNode* head) {
        if (!head) return nullptr;
        CharNode* new_head = new CharNode{ head->c, nullptr };
        CharNode* current = new_head;
        for (CharNode* p = head->next; p; p = p->next) {
            current->next = new CharNode{ p->c, nullptr };
            current = current->next;
        }
        return new_head;
    }

    void static delete_list(CharNode* head) {
        while (head) {
            CharNode* next = head->next;
            delete head;
            head = next;
        }
    }

    void print_recursive(StackItem* item, int index) {
        if (!item) return;
        print_recursive(item->next, index + 1);
        cout << index << ": ";
        for (CharNode* p = item->head; p; p = p->next)
            cout << p->c;
        cout << endl;
    }

    int static to_number(CharNode* h) {
        int val = 0, mul = 1;
        bool neg = false;
        for (; h; h = h->next) {
            if (h->c == '-') neg = true;
            else val += (h->c - '0') * mul, mul *= 10;
        }
        return neg ? -val : val;
    }

    CharNode* from_number(int val) {
        if (val == 0) return new CharNode{ '0', nullptr };
        bool neg = val < 0;
        if (neg) val = -val;
        CharNode* head = nullptr;
        while (val) {
            head = new CharNode{ char((val % 10) + '0'), head };
            val /= 10;
        }
        if (neg) {
            CharNode* p = head;
            while (p->next) p = p->next;
            p->next = new CharNode{ '-', nullptr };
        }
        return head;
    }
};

void append_char(Stack& stack, char c) {
    if (!stack.top) return;
    CharNode* node = new CharNode{ c, stack.top->head };
    stack.top->head = node;
}

void read_char(Stack& stack, char* input, int& input_pos) {
    if (!stack.top || input[input_pos] == '\0') return;
    CharNode* node = new CharNode{ input[input_pos++], stack.top->head };
    stack.top->head = node;
}

void output_char(Stack& stack) {
    if (!stack.top || !stack.top->head) return;
    cout << stack.top->head->c;
    stack.pop();
}

void run(char* program, char* input) {
    Stack stack;
    int ip = 0;
    int input_pos = 0;

    while (program[ip]) {
        int ip_before = ip;
        char instr = program[ip];
        switch (instr) {
        case '\'':
            stack.push_empty();
            break;
        case ',':
            stack.pop();
            break;
        case ':':
            stack.push_copy();
            break;
        case ';':
            stack.swap_top_two();
            break;
        case '@':
            stack.pop_number_only();
            break;
        case '&':
            stack.print();
            break;
        case '.':
            read_char(stack, input, input_pos);
            break;
        case '>':
            output_char(stack);
            break;
        case '-':
            stack.negate_number();
            break;
        case '^':
            stack.absolute_value();
            break;
        case '$':
            stack.execute_dollar();
            break;
        case '[':
            stack.pop_list_only();
            break;
        case '#':
            stack.execute_hash();
            break;
        case '~':
            stack.push_number_from_int(&ip);
            break;
        case '!':
            stack.execute_not();
            break;
        case '=':
            stack.execute_equal();
            break;
        case '<':
            stack.execute_less();
            break;
        case '?':
            stack.execute_jump(ip);
            break;
        case '+':
            stack.execute_plus();
            break;
        case ']':
            stack.execute_bracket_close();
            break;

        default:
            append_char(stack, instr);
            break;
        }
        if (ip == ip_before) ip++;
    }

   
}

int main() {

    const int MAX_PROGRAM = 20000;
    const int MAX_INPUT = 20000;
 
    // Dynamiczne wczytanie programu
    char* program = new char[MAX_PROGRAM];
    std::cin.getline(program, MAX_PROGRAM);

    // Dynamiczne wczytanie wejścia
    char* input = new char[MAX_INPUT];
    std::cin.getline(input, MAX_INPUT);

    // Uruchomienie programu
    run(program, input);

    // Zwolnienie pamięci
    delete[] program;
    delete[] input;

    


    

   
   

    return 0;
}
