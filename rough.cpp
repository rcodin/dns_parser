#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

int hex_to_dig(char hex) {
    if (hex >= '0' && hex <= '9') {
        return hex - '0';
    } else if (hex >= 'a' && hex <= 'f') {
        return 9 + (hex - 'a');
    }
    return -1;
}

// returns index of the first unprocessed character in dns_binary
int parse_qname(string &qname, string &dns_binary) {
    // get first octate
    int len = 0;
    int idx = 0;

    if (dns_binary.size() < 2)
        return;

    len = hex_to_dig(dns_binary[0]);
    len = len * 10 + hex_to_dig(dns_binary[1]);

    idx = 2;
    while (len > 0) {
        // parse the next url part
        qname = qname + dns_binary.substr(idx, len);
        idx += len;
        len = hex_to_dig(dns_binary[idx]) * 10 +
                        hex_to_dig(dns_binary[idx + 1]);
        idx += 2;
    }
    return idx;
}

int parse_aname(string &aname, string &dns_binary) {
    // read the first 2 bits

    // if pointer then 
    // pointer logic

    // label logic
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    string dns_binary;
    int n;
    

    cin>>dns_binary;
    vector<uint16_t> header;

    // 6 WORDS = 6*4 OCTATES
    if (dns_binary.size() < (6 * 4)) {
        return -1;
    }

    // parse the header section
    for (int idx = 0; idx < (6 * 4); idx += 4) {
        uint16_t word = 0;
        for (int word_octate = 0; word_octate < 4; ++word_octate)
            word = word * 16 + hex_to_dig(dns_binary[idx + word_octate]);
        dns_binary.push_back(word);
    }

    uint16_t word = dns_binary[0];
    uint16_t id;
    uint16_t qr;
    uint16_t opcode;
    uint16_t aa;
    uint16_t tc;
    uint16_t rd;
    uint16_t ra;
    uint16_t res1;
    uint16_t res2;
    uint16_t res3;
    uint16_t rcode;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;

    // 16 bits
    word = dns_binary[0];
    id = word;
    qr = word & (1<<15);
    opcode = word & (0xf<<11);
    aa = word & (1<<10);
    tc = word & (1<<9);
    rd = word & (1<<8);
    ra = word & (1<<7);
    rcode = word & 0xf;

    // 16 bits
    word = dns_binary[1];

    // 16 bits
    word = dns_binary[2];
    qdcount = word;

    // 16 bits
    word = dns_binary[3];
    ancount = word;

    // 16 bits
    word = dns_binary[4];
    nscount = word;

    // 16 bits
    word = dns_binary[5];
    arcount = word;

    dns_binary = dns_binary.substr(24);
    // parse the question section
    string qname;
    // qname parse
    int ret = parse_qname(qname, dns_binary);
    dns_binary = dns_binary.substr(ret);

    // qtype
    // 4 character string
    string qtype = dns_binary.substr(0, 4);
    dns_binary = dns_binary.substr(4);
    // qclass
    string qclass = dns_binary.substr(0, 4);
    dns_binary = dns_binary.substr(4);

    // parse the answer section
    string answer_name;
    string answer_type;
    string answer_class;
    string answer_ttl;
    string answer_rlength;
    string answer_rdata;

    // parse answer name
    // type data structure


    return 0;
}

// a01d81800001000100000000

// 076578616d706c6503636f6d0000010001

// c00c0001000100001bbc00045db8d822