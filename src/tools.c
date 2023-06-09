#include "main.h"
#include "api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool setupFileLocation(const char *fileLocation)
{
  FILE *fileBuffer;

  if(!(fileBuffer = fopen(fileLocation, "r")))
    return false;

  fclose(fileBuffer);

  return true;
}

int getFileInfo(char *fileKey, u8 fileType, const char *fileLocation)
{
  FILE *fileBuffer = fopen(fileLocation, "r");
  char buffer[255], value[48];
  enum FileTypes type = (enum FileTypes)fileType;

  switch(type)
  {
    case CLIENT_ID:
      strcpy(fileKey, "client-id");
    break;
    case BEARER_TOKEN:
      strcpy(fileKey, "bearer-token");
    break;
    case OAUTH_TOKEN:
      strcpy(fileKey, "oauth-token");
    break;
    default: break;
  }

  while(fgets(buffer, sizeof(buffer), (FILE*)fileBuffer))
  {
    bool collectKey = false;
    bool stayForEver = true;
    int i = 0, ii = 0;

    while(stayForEver)
    {
      switch(buffer[i])
      {
        case '#': stayForEver = false; break;
        case ' ': break;
        case '=': collectKey = true; break;
        case ',':
        {
          if(!strncmp(buffer, fileKey, strlen(fileKey))) strncpy(fileKey, value, strlen(value));
          stayForEver = false;
        }
        break;
        default:
        {
          if(collectKey)
          {
            value[ii] = buffer[i];
            ii++;
          }
        }
        break;
      }
      i++;
    }
  }
  fclose(fileBuffer);

  return 0;
}
