#import <QObject>
#ifndef CAR_CAN_H
#define CAR_CAN_H

class SubMessage
{
	public: 
		SubMessage(int num, QString text, bool print_if_low);
		int num;
		QString text;
		bool print_if_low;
};

class Message {
	public:
		Message(QString text, int multiplier, enum units the_units, int num_nibbles, bool isSigned);
		int get_true_value();
		
		QString text;
		int multiplier;
		enum units {MPH, VOLTS, WATTS, JOULES, PERCENT};
		enum units the_units; //check this, it might have to go in a constructor or something
		int num_nibbles;
		bool isSigned;
		QList<SubMessage> sub_messages; //idk if this will work, I may have to do some rearranging of things to implement SubMessage
		int value; //not sure about the type for this
};

//global report; //not sure what to do with this
//report[this.text] = this; //or this

//std::unordered_map<QString, Message> report; //I think this is what it's gonna have to be		

			  
class CAN{
	public:
		CAN{
			self.closing = False; // Set to true if we are trying to close the program
	
			// Delete last log file if there are more than 9
	
			files = glob.glob('Logs/*.txt');
	
			while (len(files) >= 10) {
	
				os.remove(sorted(files)[0]);
	
				files = glob.glob('*.txt');
			}
	
			// Create our log file
	
			self.log_file = open('Logs/' + str(datetime.datetime.now()) + '.txt', 'w');
	
	
			// More shit that is exclusive to the pi
	
			if (running_on_pi) {
	
				GPIO.setmode(GPIO.BCM);
	
				GPIO.setup(15, GPIO.IN);
			}
	
			// Initialize serial communication to CANdapter
	
			self.initSerial();
	
			// Start a loop for getting speed
	
			"""s = threading.Thread(target=self.getSpeed)
	
			s.daemon = True;
	
			s.start();"""
	
			// Save default information to our log (mainly for debugging)
	
			self.log();
			
		}
};

Struct report{};

Struct id{};

class car_CAN
{
	public:
		car_CAN();
		QList<SubMessage> current_limit_status;
		QList<SubMessage> relay_status;
		QList<id> ids;
		bool running_on_pi;
		int max_files_to_keep;
		
		QString text;
		bool print_if_low;
		
		void bytes_to_int(bytes(?) bts, bool is_signed);
		void bits_from_int(int i);
		
};


#endif // CAR_CAN_H
