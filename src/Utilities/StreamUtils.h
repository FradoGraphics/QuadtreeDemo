/**
 * Class name:    StreamUtils

 * Description:   This helper class facilitates working with string streams. 

 * Author:        Frank Dockhorn
 */


#ifndef STREAM_UTILS
#define STREAM_UTILS

class StreamUtils
{
public:
	static void	resetStream(tstringstream& stream)
	{
		stream.str(tstring());
		stream.clear();
	}
	static void	resetStreamA(astringstream& stream)
	{
		stream.str(astring());
		stream.clear();
	}		
};

#endif