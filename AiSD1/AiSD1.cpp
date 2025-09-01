#include <iostream>
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

    void execute_at() {
        if (!top) return;
        int index = to_number(top->head);
        pop();
        StackItem* item = get_nth_item(index);
        if (!item) return;
        CharNode* copied = copy_list(item->head);
        StackItem* new_item = new StackItem{ copied, top };
        top = new_item;
    }

    void absolute_value() {
        if (!top || !top->head) return;
        CharNode* prev = nullptr;
        CharNode* curr = top->head;
        while (curr->next) {
            prev = curr;
            curr = curr->next;
        }
        if (curr->c == '-') {
            if (prev)
                prev->next = nullptr;
            else
                top->head = nullptr;
            delete curr;
        }
    }

    void negate_number() {
        if (!top) return;
        CharNode* curr = top->head;
        CharNode* prev = nullptr;
        if (!curr) {
            top->head = new CharNode{ '-', nullptr };
            return;
        }
        while (curr->next) {
            prev = curr;
            curr = curr->next;
        }
        if (curr->c == '-') {
            if (prev)
                prev->next = nullptr;
            else
                top->head = nullptr;
            delete curr;
        }
        else {
            curr->next = new CharNode{ '-', nullptr };
        }
    }

    void execute_dollar() {
        CharNode* ch = detach_first_char();
        if (ch) push_list(ch);
    }

    void execute_bracket_open() {
        if (!top || !top->head) {
            pop();
            return;
        }
        char c = top->head->c;
        pop();
        push_list(from_number(static_cast<int>(c)));
    }

    void static execute_bracket_close(Stack& stack) {
        if (!stack.top) return;
        int number;
        try {
            number = Stack::to_number(stack.top->head);
        }
        catch (...) {
            return;
        }
        stack.pop();
        char c = static_cast<char>(number);
        CharNode* node = new CharNode{ c, nullptr };
        stack.push_list(node);
    }

    void execute_hash() {
        if (!top || !top->next) return;
        StackItem* itemA = top;
        CharNode* listA = itemA->head;
        top = top->next;
        delete itemA;
        if (!top->head) {
            top->head = listA;
            return;
        }
        CharNode* curr = top->head;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = listA;
    }

    void push_number_from_int(const int* n_ptr) {
        if (!n_ptr) return;
        int val = *n_ptr;
        bool is_negative = false;
        if (val < 0) {
            is_negative = true;
            val = -val;
        }
        if (val == 0) {
            push_list(new CharNode{ '0', nullptr });
            return;
        }
        CharNode* head = nullptr;
        CharNode* tail = nullptr;
        while (val > 0)
        {
            CharNode* node = new CharNode{ char((val % 10) + '0'), nullptr };
            if (!head)
                head = tail = node;
            else {
                tail->next = node;
                tail = node;
            }
            val /= 10;
        }
        if (is_negative) {
            tail->next = new CharNode{ '-', nullptr };
        }
        push_list(head);
    }

    void execute_not() {
        if (!top) return;
        if (!top->head) {
            delete_list(top->head);
            top->head = new CharNode{ '1', nullptr };
            return;
        }
        if (top->head->c == '0' && top->head->next == nullptr) {
            delete_list(top->head);
            top->head = new CharNode{ '1', nullptr };
        }
        else {
            delete_list(top->head);
            top->head = new CharNode{ '0', nullptr };
        }
    }

    void execute_equal() {
        if (!top || !top->next) return;
        bool A_is_number = is_number(top->head);
        bool B_is_number = is_number(top->next->head);
        bool eq;
        if (A_is_number && B_is_number) {
            int A = to_number(top->head);
            int B = to_number(top->next->head);
            eq = (A == B);
        }
        else {
            eq = lists_equal(top->head, top->next->head);
        }
        pop(); pop();
        push_list(new CharNode{ char((eq ? '1' : '0')), nullptr });
    }

    void execute_less() {
        if (!top || !top->next) return;
        bool A_is_number = is_number(top->head);
        bool B_is_number = is_number(top->next->head);
        bool result;
        if (A_is_number && B_is_number) {
            int A = to_number(top->head);
            int B = to_number(top->next->head);
            result = B < A;
        }
        else {
            CharNode* a = top->next->head;
            CharNode* b = top->head;
            while (a && b && a->c == b->c) {
                a = a->next;
                b = b->next;
            }
            if (!a && b) result = true;
            else if (a && !b) result = false;
            else if (!a && !b) result = false;
            else result = a->c < b->c;
        }
        pop(); pop();
        push_list(new CharNode{ char((result ? '1' : '0')), nullptr });
    }

    void execute_jump(int& ip) {
        if (!top || !top->next) return;
        int T = to_number(top->head);
        CharNode* W = top->next->head;
        bool jump = W && !(W->c == '0' && W->next == nullptr);
        pop(); pop();
        if (jump) ip = T;
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
        long long val = 0;
        bool neg = false;
        int digits[150];
        int count = 0;
        for (; h; h = h->next) {
            if (h->c == '-') neg = true;
            else if (h->c >= '0' && h->c <= '9') {
                if (count < 150)
                    digits[count++] = h->c - '0';
                else
                    break;
            }
        }
        for (int i = count - 1; i >= 0; --i) {
            val = val * 10 + digits[i];
            if (val > INT32_MAX) return 0;
        }
        return neg ? -int(val) : int(val);
    }

    bool static is_number(CharNode* h) {
        if (!h) return false;
        bool has_digit = false;
        int digit_count = 0;
        for (; h; h = h->next) {
            if (h->c >= '0' && h->c <= '9') {
                has_digit = true;
                digit_count++;
                if (digit_count > 18) return false;
            }
            else if (h->c != '-') return false;
        }
        return has_digit;
    }

    bool static lists_equal(CharNode* a, CharNode* b) {
        while (a && b) {
            if (a->c != b->c) return false;
            a = a->next;
            b = b->next;
        }
        return !a && !b;
    }

    StackItem* get_nth_item(int n) {
        StackItem* curr = top;
        for (int i = 0; i < n && curr; ++i)
            curr = curr->next;
        return curr;
    }

    CharNode* from_number(int val) {
        if (val == 0) return new CharNode{ '0', nullptr };
        bool neg = val < 0;
        if (neg) val = -val;
        CharNode* head = nullptr;
        CharNode* tail = nullptr;
        while (val) {
            CharNode* node = new CharNode{ char((val % 10) + '0'), nullptr };
            if (!head)
                head = tail = node;
            else {
                tail->next = node;
                tail = node;
            }
            val /= 10;
        }
        if (neg) {
            tail->next = new CharNode{ '-', nullptr };
        }
        return head;
    }
};

void append_char(Stack& stack, char c) {
    if (!stack.top) return;
    CharNode* node = new CharNode{ c, stack.top->head };
    stack.top->head = node;
}

void output_char(Stack& stack) {
    if (!stack.top) return;
    CharNode* head = stack.top->head;
    if (head)
        cout << head->c;
    stack.pop();
}

void read_char(Stack& stack, char* input, int& input_pos) {
    if (!stack.top) return;
    if (!input[input_pos]) return;
    char c = input[input_pos];
    input_pos++;
    append_char(stack, c);
}

void run(char* program, char* input) {
    Stack stack;
    int ip = 0;
    int input_pos = 0;

    while (program[ip]) {
        int ip_before = ip;
        int ip_for_tilde = ip;
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
            stack.execute_at();
            break;
        case '&':
            stack.print();
            break;
        case '^':
            stack.absolute_value();
            break;
        case '-':
            stack.negate_number();
            break;
        case '>':
            output_char(stack);
            break;
        case '.':
            read_char(stack, input, input_pos);
            break;
        case '$':
            stack.execute_dollar();
            break;
        case ']':
            stack.execute_bracket_close(stack);
            break;
        case '[':
            stack.execute_bracket_open();
            break;
        case '#':
            stack.execute_hash();
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
        case '~':
            stack.push_number_from_int(&ip_for_tilde);
            break;
        case '!':
            stack.execute_not();
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

    char* program = new char[MAX_PROGRAM];
    cin.getline(program, MAX_PROGRAM);

    char* input = new char[MAX_INPUT];
    cin.getline(input, MAX_INPUT);

    run(program, input);

    delete[] program;
    delete[] input;

    return 0;
}
