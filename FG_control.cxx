#include "FG_control.hxx"

int SocketConnect(){
	int socketOsc;
	struct sockaddr_in serverAddr;

	//create socket
	std::cout<<"Creating socket..."<<std::endl;
	socketOsc = socket(AF_INET, SOCK_STREAM, 0);
	if(socketOsc == -1){
		std::cerr<<"Failed to create socket"<<std::endl;
		exit(-1);
	}
	std::cout<<"Created socket SID = "<<socketOsc<<std::endl;
	
	//socket connect
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = inet_addr(host_ip);

	std::cout<<"Connecting to remote host..."<<std::endl;
	if( connect(socketOsc, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		std::cerr<<"Failed to connect to remote host"<<std::endl;
		exit(-1);
	}

	return socketOsc;

}

void SendCommand(int socketOsc, char const* cmd){
	//send command
	std::cout<<"Sending command: "<<cmd<<std::endl;
	if( send(socketOsc, cmd, strlen(cmd), 0) <= 0){
		std::cerr<<"Failed to send command"<<std::endl;
		exit(-1);
	}
}

void SocketQuery(int socketOsc, char const* cmd){
	char recv_str[4096];
	//send command
	SendCommand(socketOsc, cmd);
        
	//receive reponse from remote host
	memset(recv_str, 0, sizeof(recv_str));         //memory initialize
	std::cout<<"Receiving reponse..."<<std::endl;
	if( recv(socketOsc, recv_str, sizeof(recv_str), 0) <= 0){
		std::cerr<<"Failed to receive response"<<std::endl;
		exit(-1);
	}
	std::cout<<recv_str<<std::endl;

	close(socketOsc);
}

std::string TranslateCommand(int ch, std::string mode, std::string cmd){
	if(mode == "-q" || mode == "--query"){
		if(cmd == "identify")          return "*IDN?\n";
		else if (cmd == "waveList?")   return "STL?\n";

		else if (cmd == "output?")     return "C" + std::to_string(ch) + ":OUTPut?\n";
		else if (cmd == "waveInfo?")   return "C" + std::to_string(ch) + ":BSWV?\n";
		else if (cmd == "modInfo?")    return "C" + std::to_string(ch) + ":MDWV?\n";
		else{
			std::cerr<<"no such command!"<<std::endl;
			exit(-1);
		}
	}
	else{
		std::cerr<<"no such mode!"<<std::endl;
		exit(-1);
	}
}

std::string TranslateCommand(int ch, std::string mode, std::string cmd, std::string parameter){
	if(mode == "-s" || mode == "--send"){
		if(cmd == "outputLoad")        return "C" + std::to_string(ch) + ":OUTPut LOAD," + parameter + "\n";
		else if(cmd == "outputSwitch") return "C" + std::to_string(ch) + ":OUTPut " + parameter + "\n";

		else if(cmd == "waveType")     return "C" + std::to_string(ch) + ":BSWV WVTP," + parameter + "\n";
		else if(cmd == "frequency")    return "C" + std::to_string(ch) + ":BSWV FRQ," + parameter + "\n";
		else if(cmd == "period")       return "C" + std::to_string(ch) + ":BSWV PERI," + parameter + "\n";
		else if(cmd == "amplitude")    return "C" + std::to_string(ch) + ":BSWV AMP," + parameter + "\n";
		else if(cmd == "offset")       return "C" + std::to_string(ch) + ":BSWV OFST," + parameter + "\n";
		else if(cmd == "symmetry")     return "C" + std::to_string(ch) + ":BSWV SYM," + parameter + "\n";
		else if(cmd == "duty")         return "C" + std::to_string(ch) + ":BSWV DUTY," + parameter + "\n";
		else if(cmd == "phase")        return "C" + std::to_string(ch) + ":BSWV PHSE," + parameter + "\n";
		else if(cmd == "stdev")        return "C" + std::to_string(ch) + ":BSWV STDEV," + parameter + "\n";
		else if(cmd == "mean")         return "C" + std::to_string(ch) + ":BSWV MEAN," + parameter + "\n";
		else if(cmd == "pulseWidth")   return "C" + std::to_string(ch) + ":BSWV WIDTH," + parameter + "\n";
		else if(cmd == "riseTime")     return "C" + std::to_string(ch) + ":BSWV RISE," + parameter + "\n";
		else if(cmd == "fallTime")     return "C" + std::to_string(ch) + ":BSWV FALL," + parameter + "\n";
		else if(cmd == "delay")        return "C" + std::to_string(ch) + ":BSWV DLY," + parameter + "\n";
		else if(cmd == "highLevel")    return "C" + std::to_string(ch) + ":BSWV HLEV," + parameter + "\n";
		else if(cmd == "lowLevel")     return "C" + std::to_string(ch) + ":BSWV LLEV," + parameter + "\n";

		else if(cmd == "moduleSwitch") return "C" + std::to_string(ch) + ":MDWV STATE," + parameter + "\n";
		else if(cmd == "moduleSelect") return "C" + std::to_string(ch) + ":MDWV " + parameter + "\n";
		else if(cmd == "pwmSource")    return "C" + std::to_string(ch) + ":MDWV PWM,SRC," + parameter + "\n";
		else if(cmd == "pwmFrequency") return "C" + std::to_string(ch) + ":MDWV PWM,FRQ," + parameter + "\n";
		else if(cmd == "pwmDuty")      return "C" + std::to_string(ch) + ":MDWV PWM,DEVI," + parameter + "\n";
		else if(cmd == "pwmShape")     return "C" + std::to_string(ch) + ":MDWV PWM,MDSP," + parameter + "\n";

		else if(cmd == "arbWaveType")  return "C" + std::to_string(ch) + ":ARWV NAME," + parameter + "\n";
		else if(cmd == "arbWaveIndex") return "C" + std::to_string(ch) + ":ARWV INDEX," + parameter + "\n";

		else if(cmd == "arbWaveSave"){
			std::ifstream f_in(parameter, std::ios::in);
			if(!f_in){ std::cerr<<"can not find waveform file!"<<std::endl; exit(-1); }
			std::string wavename;
			std::string point;
			std::string data;
			f_in>>wavename;
			while(f_in>>point){
				int16_t num = stoi(point, 0, 16);
				data += reinterpret_cast<char const*>(&num);
				std::cout<<std::hex<<(int) data[1]<<std::endl;
			}
			f_in.close();
			return "C" + std::to_string(ch) + ":WVDT WVNM," + wavename + ",WAVEDATA," + data + "\n";
		}

		else{ std::cerr<<"no such command!"<<std::endl; exit(-1); }
	}
	else{ std::cerr<<"no such mode!"<<std::endl; exit(-1); }

}

void Usage(){
	std::cout<<"./FG_control <channel> <mode> <command> <parameter>"<<std::endl;
	std::cout<<"mode <\033[32m-q\033[0m> or <\033[32m--query\033[0m>: get informations (no parameter needed)"<<std::endl;
	std::cout<<"notice that for the last questionare sign you need to put a \\ before it"<<std::endl;
	std::cout<<"     command <\033[32moutput?\033[0m> returns channel output information"<<std::endl;
	std::cout<<"     command <\033[32mwaveInfo?\033[0m> returns channel waveform information"<<std::endl;
	std::cout<<"     command <\033[32mmodInfo?\033[0m> returns channel Mod information"<<std::endl;
	std::cout<<"mode <\033[32m-s\033[0m> or <\033[32m--send\033[0m>: send commands to F.G. (need parameter after command)"<<std::endl;
	std::cout<<"     command <\033[32moutputLoad\033[0m> set Load resistance [Ohm] or HZ"<<std::endl;
	std::cout<<"     command <\033[32moutputSwitch\033[0m> \033[33mon/off\033[0m the output"<<std::endl;
	std::cout<<"     command <\033[32mwaveType\033[0m> set wave shape to \033[33msine/square/ramp/pulse/noise/arb/dc/prbs\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mfrequency\033[0m> set frequency [Hz]"<<std::endl;
	std::cout<<"     command <\033[32mperiod\033[0m> set period [sec]"<<std::endl;
	std::cout<<"     command <\033[32mamplitude\033[0m> set amplitude [V]"<<std::endl;
	std::cout<<"     command <\033[32moffset\033[0m> set offset [V]"<<std::endl;
	std::cout<<"     command <\033[32msymmetry\033[0m> set symmetry [%], \033[33monly for square and pulse waves\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mduty\033[0m> set duty factor [%], \033[33monly for square and pulse waves\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mphase\033[0m> set phase [0, 360], \033[33mNOT valid for noise/pulse/dc\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mstdev\033[0m> set standard deviation, \033[33monly for noise wave\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mmean\033[0m> set mean value, \033[33monly for noise wave\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mpulseWidth\033[0m> set pulse width, \033[33monly for pulse wave\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mriseTime\033[0m> set rise time [sec], \033[33monly for pulse wave\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mfallTime\033[0m> set fall time [sec], \033[33monly for pulse wave\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mdelay\033[0m> set signal delay [sec], \033[33monly for pulse wave\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mhighLevel\033[0m> set high level [V], \033[33mNOT valid for noise/dc\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mlowLevel\033[0m> set low level [V], \033[33mNOT valid for noise/dc\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mmoduleSwitch\033[0m> switch on/off the Mod"<<std::endl;
	std::cout<<"     command <\033[32mmoduleSelect\033[0m> select the modulation to \033[33mam/dsbam/fm/pm/pwm/ask/fsk/carr\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mpwmSource\033[0m> pwm signal source \033[33mint/ext\033[0m"<<std::endl;
	std::cout<<"     command <\033[32mpwmFrequency\033[0m> pwm frequency [Hz]"<<std::endl;
	std::cout<<"     command <\033[32mpwmDuty\033[0m> pwm duty factor [%]"<<std::endl;
	std::cout<<"     command <\033[32mpwmShape\033[0m> set pwm wave shape to \033[33msine/square/triangle/upramp/dnramp/noise/arb\033[0m"<<std::endl;
}

int main(int argc, char** argv){
	if(argc <4 || argc > 5){
		Usage();
		exit(-1);
	}
	int ch = atoi(argv[1]);
	std::string mode = argv[2];
	std::string cmd;
	int socketOsc = SocketConnect();

	std::cout<<"mode = "<<mode << std::endl;
	if(mode == "-q" || mode == "--query"){
		std::cout<<"Query mode"<<std::endl;
		cmd = TranslateCommand(ch, mode, argv[3]);
		SocketQuery(socketOsc, cmd.c_str());
	}
	else if(mode == "-s" || mode == "--send"){
		if(argc != 5){
			std::cerr<<"need parameter for this mode!"<<std::endl;
			exit(-1);
		}
		cmd = TranslateCommand(ch, mode, argv[3], argv[4]);
		std::cout<<"Send command to remote host mode"<<std::endl;
		SendCommand(socketOsc, cmd.c_str());
	}
	else if(mode == "-h" || mode == "--help"){
		Usage();
		exit(-1);
	}
	else{
		std::cerr<<"no such mode!"<<std::endl;
		std::cerr<<"use -h or --help argument to see help message"<<std::endl;
		exit(-1);
	}
		

	return 0;
}
