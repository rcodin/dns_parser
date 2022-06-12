enum MESSAGE_TYPE {
	TYPE_LABEL,
	TYPE_POINTER
};

class Utils {
public:
	int hex_to_dig(char hex) {
		if (hex >= '0' && hex <= '9') {
			return hex - '0';
		} else if (hex >= 'a' && hex <= 'f') {
			return 9 + (hex - 'a');
		}
		return -1;
	}

	// return the end index
	int labels_parser(string &message, int &start_index,
									string &output) {
		// get first octate
		int len = 0;
		int idx = start_index;

		if (message.size() < 2)
			return;

		len = hex_to_dig(message[idx]);
		len = len * 10 + hex_to_dig(message[idx + 1]);

		idx += 2;
		while (len > 0) {
			// parse the next url part
			qname = qname + message.substr(idx, len);
			idx += len;
			len = hex_to_dig(message[idx]) * 10 +
							hex_to_dig(message[idx + 1]);
			idx += 2;
		}

		output = qname;
		return idx;
	}

	int pointer_parser(string &message, int &start_index,
									int &output) {
		int temp = 0;
		// ignore first 2 bits

		temp = temp * 16 + (hex_to_dig(message[start_index]) && (3<<2));
		temp = temp * 16 + hex_to_dig(message[start_index + 1]);
		temp = temp * 16 + hex_to_dig(message[start_index + 2]);
		temp = temp * 16 + hex_to_dig(message[start_index + 3]);

		output = temp;

		return start_index + 4;
	}

	enum MESSAGE_TYPE decode_message_type(string &message, int &start_index){
		int code = -1;
		enum MESSAGE_TYPE type;

		code = message[start_index] & 3;

		switch(code) {
			case 0:
				type = TYPE_LABEL;
				break;
			case 3:
				type = TYPE_POINTER;
				break;
			default:
				break;
		}
		return type;
	}
	// utility to print ip
	
	// utility to print domain name
};