#if defined(FMX_WIN_TARGET)
//	#include <windows.h>
	#include <locale.h>
#endif

#if defined(FMX_MAC_TARGET)
	#include <CoreServices/CoreServices.h>
	#include <Carbon/Carbon.h>
#endif 

#include "FMPlugin.h"
#include "FMTemplate/FMTemplate.h"
#include "Base64.h"
//#include "StringUtils.h"

#include <inttypes.h>
#include <string>
using namespace std;

#define Min(x,y)	(((x) < (y)) ? (x) : (y))

namespace base64
{

const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char base64pad = '=';

/*
 * @brief Encode a buffer on a string base64 encoded
 * 
 * Base 64 encoding converts 3 bytes into 4 encoded ASCII 
 * characters.
 * If the encoded string lenght is not multiple of 4
 * the padding character "=" is added for fill the buffer.
 *  
 * @param buf_ptr pointer to the source buffer
 * @param buf_size size of the buffer
 * 
 * @return a string base64 encoded
 * 
 * @see http://en.wikipedia.org/wiki/Base64
 */

std::string encode(uint8_t* buf_ptr, size_t buf_size)
{
   uint32_t n = 0;
   uint8_t n0, n1, n2, n3;
   std::stringstream ss_result;
   
   // Iterate over the buffer
   for (size_t x = 0; x < buf_size; x += 3) 
   {
      // Get 3 bytes of 8 bit from the buffer 
      n = buf_ptr[x] << 16;
 
      if ((x+1) < buf_size)
      {
          n += buf_ptr[x+1] << 8;
 
          if ((x+2) < buf_size)
          {
             n += buf_ptr[x+2];
          }
      }
      
      // Transform on 4 bytes of 6 bit
      n0 = (uint8_t)(n >> 18) & 63;
      n1 = (uint8_t)(n >> 12) & 63;
      n2 = (uint8_t)(n >> 6) & 63;
      n3 = (uint8_t)n & 63;
 
      // Write first 2 bytes into the stream
      ss_result << base64chars[n0];
      ss_result << base64chars[n1];

      // Test if only 2 bytes
      if ((x+1) < buf_size)
      {
          // Write 3rd into the stream
          ss_result << base64chars[n2];
 
          // Test if only 3 bytes
          if ((x+2) < buf_size)
          {
              // Write 4th into the stream
              ss_result << base64chars[n3];
          }
      }
   }  
 
   // Add padding character for fill the output string
   for (int pad_count = buf_size % 3; pad_count < 3; pad_count++) 
   { 
       ss_result << base64pad;
   } 
   
   return ss_result.str();
}

/**
 * @brief Decode a string base64 encoded and return a raw buffer
 * 
 * The function allocate a buffer with size: 
 * @(text.size() + 3) / 4 * 3
 * and return the pointer. The caller has the ownership of the
 * pointer.
 * The source string lenght must be multiple of 4.
 *  
 * @param text the base64 string encoded
 * 
 * @return the pointer to the decoded buffer
 */
std::auto_ptr< uint8_t > decode(const std::string& text)
{
    // Create the decode matrix
    uint8_t decode_base64chars[256];
    memset (decode_base64chars, 0, 256);
    
    for (uint8_t i = 0; i <= 63; i++)
    {
        decode_base64chars[(int)base64chars[(int)i]] = i;
    }

    // Allocate output buffer
    uint8_t* buf_ptr = new guint8[(text.size() + 3) / 4 * 3];
    
    // Start decode source string
    size_t buf_idx = 0;
    for (size_t i = 0; i < text.size(); i += 4, buf_idx += 3)
    {
        // Put first base64 char on the first output-byte
        buf_ptr[buf_idx] = decode_base64chars[(int)text[i]] << 2;
        
        // Test for string end
        if ((i + 1 < text.size()) && (text[i + 1] != base64pad))
        {
            // Add to the first output-byte 2 bit of the second base64 char
            buf_ptr[buf_idx] += (decode_base64chars[(int)text[i + 1]] >> 6) & 0x3;
        
            // Put remaining 4 bit of the second base64 char on the second output-byte
            buf_ptr[buf_idx + 1] = decode_base64chars[(int)text[i + 1]] << 4;
            
            // Test for string end
            if ((i + 2 < text.size()) && (text[i + 2] != base64pad))
            {
                // Add to the second output-byte 4 bit of the third base64 char
                buf_ptr[buf_idx + 1] += (decode_base64chars[(int)text[i + 2]] >> 4) & 0xf;
                
                // Put remaining 2 bit of the third base64 char on the third output-byte
                buf_ptr[buf_idx + 2] = decode_base64chars[(int)text[i + 2]] << 6;
                
                // Test for string end
                if ((i + 1 < text.size()) && (text[i + 3] != base64pad))
                {
                    // Add to the third output-byte the forth base64 char
                    buf_ptr[buf_idx + 2] += decode_base64chars[(int)text[i + 3]];
                }
            }
        }
    }
    
    return std::auto_ptr<uint8_t>(buf_ptr);
}

};

