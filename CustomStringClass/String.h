#ifndef STRING_H
#define STRING_H

class String {
    public: 
        String(); 
        String(char[], const int);
        String(const String &);
        ~String(); 
       
        int length() const; 
        void clear();
        bool empty() const;
        char at(const int) const;
        int find(const String&, const int) const;
        bool equal(const String &) const;
        bool add(const String &);
        void print() const;
        int count(const String &) const; 
    protected: 

    private: 
        int size; 
        char *word; 
};

#endif // STRING_H