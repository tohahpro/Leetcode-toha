#include<iostream>
using namespace std;
class Arra {
    int* arr;
    int size;
public:
    Arra(int n) {
        size = n;   
        arr = new int[size];
    }   
    void input() {
        cout << "Enter the elements of array: ";
        for(int i = 0; i < size; i++) {
            cin >> arr[i];
        }
    }
    void display() {
        cout << "The elements of array are: ";
        for(int i = 0; i < size; i++) {
            cout << arr[i] << " ";  
        }
        cout << endl << endl;
    }
    void sort() {
        for(int i = 0; i < size - 1; i++) {
            for(int j = 0; j < size - i - 1; j++) {
                if(arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
    ~Arra() {
        delete[] arr;
    }
};
int main() {
    int n;      

    cout << "Enter the size of array: ";
    cin >> n;               

    Arra array(n);
    array.input();
    array.sort();
    array.display();
    return 0;
}       
    