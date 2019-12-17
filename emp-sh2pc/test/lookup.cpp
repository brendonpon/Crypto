#include "emp-sh2pc/emp-sh2pc.h"
using namespace emp;
using namespace std;

void test_lookup(int list_length, int bitsize, string inputs_a[], string inputs_a2[], string inputs_b[]) {
  	Integer result(bitsize, 0, PUBLIC);


	Integer a[list_length];
	Integer a2[list_length];
	Integer b;
	for (int i=0; i<list_length; i++) {
    	   a[i] = Integer(bitsize, inputs_a[i], ALICE);
    	   a2[i] = Integer(bitsize, inputs_a2[i], ALICE);
	   //b[i] = Integer(bitsize, inputs_b[i], BOB);
	}
	b = Integer(bitsize, inputs_b[0], BOB);
	for( int i=0; i < list_length; i++ ) {
	   Bit s = a[i].equal(b);
           //cout << s.reveal();
           //cout << a[i].reveal<int>();
           //cout << b.reveal<int>();
           result =result.select(s, a2[i]);
    		// reconstruct "secret shared" inputs
    		//a[i] = a[i] + b[i];
    		// multiply value into product
    		//product = product * a[i];
  	}

  	cout << "Result (binary notation): ";
  	for(int i=bitsize-1; i>=0; i--) {
  	    cout << result[i].reveal();
  	}
  	cout << endl;
}

int main(int argc, char** argv) {
    int bitsize;

    // generate circuit for use in malicious library
    if (argc == 3 && strcmp(argv[1], "-m") == 0 ) {
        setup_plain_prot(true, "mult3.circuit.txt");
        bitsize = 16;
	int list_length = atoi(argv[2]);
        string inputs[list_length];
        string inputs_b[1] = {0};
	for (int i =0; i<list_length; i++) {
	    inputs[i] = to_string(0);
	}
        test_lookup(list_length, bitsize, inputs, inputs, inputs_b);
        finalize_plain_prot();
	return 0;
    }
    
    // run computation with semi-honest model
    int port, party;
    parse_party_and_port(argv, &party, &port);
    NetIO * io = new NetIO(party==ALICE ? nullptr : "127.0.0.1", port);

    setup_semi_honest(io, party);

    if (argc != 5) {
      cout << "Usage: ./lookup <party> <port> <bitsize> <length>" << endl
           << "where <value> are secret shares of the three inputs"
           << endl;
      delete io;
      return 0;
    }
    int list_length = 10;
    if (argc == 5) {
	list_length = atoi(argv[4]);
    }
    bitsize = atoi(argv[3]);
    char fname_a[40];
    char fname_b[40];
    char fname_a2[40];
    sprintf(fname_a, "../data/lookup/%d.1.dat", bitsize);
    sprintf(fname_b, "../data/lookup/%d.2.dat", bitsize);
    sprintf(fname_a2, "../data/lookup/%d.1_2.dat", bitsize);
    ifstream infile_a(fname_a);
    ifstream infile_b(fname_b);
    ifstream infile_a2(fname_a2);
    string inputs_a[list_length];
    string inputs_a2[list_length];
    string inputs_b[1];

    if( infile_a.is_open() && infile_a2.is_open() && infile_b.is_open()) {
        getline(infile_b, inputs_b[0]);
	for( int i=0; i<list_length; i++) {
            getline( infile_a, inputs_a[i]);
	    getline( infile_a2, inputs_a2[i]);
            //getline( infile_b, inputs_b[i]);
        }
        infile_a.close();
        infile_b.close();
    }

    test_lookup(list_length, bitsize, inputs_a, inputs_a2, inputs_b);
    delete io;
}

