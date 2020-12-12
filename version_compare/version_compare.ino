/*
   Software versions compare tool for Arduino.
   Tested on Nano 33 IoT.

   Created on: December, 2020
   License: MIT license.
   Author: Nicola Elia
*/

// #define DEBUG // Uncomment to enable debug prints

void setup() {
  // Initialize serial and wait for connection
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
}

void loop() {
  // Versions to compare, as char arrays
  char a[] = "3.14.0.1";
  char b[] = "3.15";
  
  short comparison = versionCompare(a, b);
  
  if (comparison == 2)
  {
    Serial.println(F("a, b are the same version!"));
  }
  else if (comparison == 0)
  {
    Serial.println(F("a is a newer version!"));
  }
  else if (comparison == 1)
  {
    Serial.println(F("b is a newer version!"));
  }
  else
  {
    Serial.println(F("Error comparing versions!"));
  }

  while (1) ;
}

short versionCompare(char v1[], char v2[]) {
  /* Compares two version strings.

     Versions can have up to 5 sections, and each section can span from 0 to 65535:
     from '0' to '65535.65535.65535.65535.65535'

     Returns 0 if v1 > v2
     Returns 1 if v2 > v1
     Returns 2 if v1 = v2
     Returns 3 in case of errors            */

  if ((strlen(v1) > 30) | (strlen(v2) > 30))
  {
    Serial.println(F("[versionCompare] Error: too long input strings."));
    return 3;
  }
  short max_size = 0;
  if (strlen(v2) > strlen(v1)) {
    max_size = strlen(v2); // Equals to (sizeof(str) - 1)
  } else {
    max_size = strlen(v1);
  }

  short v1_s = versionCountSections(v1);
  short v2_s = versionCountSections(v2);
  short n_s = (v1_s > v2_s) ? v1_s : v2_s;

  unsigned short v1_int[n_s] = {0};
  unsigned short v2_int[n_s] = {0};
  for (short s = 0; s < v1_s; s++) {
    v1_int[s] = extractSection(v1, v1_s, s);
  }
  for (short s = 0; s < v2_s; s++) {
    v2_int[s] = extractSection(v2, v2_s, s);
  }

#ifdef DEBUG
  // Debug the conversion to int arrays
  Serial.print(F("v1_int:\t"));
  for (short s = 0; s < n_s; s++) {
    Serial.print(v1_int[s]);
    Serial.print(F("\t"));
  }
  Serial.println();
  Serial.print(F("v2_int:\t"));
  for (short s = 0; s < n_s; s++) {
    Serial.print(v2_int[s]);
    Serial.print(F("\t"));
  }
  Serial.println();
#endif

  short comp[n_s] = {};
  for (short s = 0; s < n_s; s++)
  {
    // Compare each section
    if (v1_int[s] < v2_int[s])
    {
      comp[s] = 0;
    } else if (v1_int[s] == v2_int[s])
    {
      comp[s] = 1;
    } else if (v1_int[s] > v2_int[s])
    {
      comp[s] = 2;
    }
  }

  for (short s = 0; s < n_s; s++)
  {
    if (comp[s] == 2)
    {
      // First comparison, or previous section with same version
      // v1 section version is bigger --> v1 > v2
      return 0;
    }
    else if (comp[s] == 0)
    {
      // First comparison, or previous section with same version
      // v2 section version is bigger --> v2 > v1
      return 1;
    }
    else
    {
      // Current section has the same version, is it the last one?
      if (s == (n_s - 1))
      {
        // v1 and v2 are the same version
        return 2;
      }
    }
  }
}

short versionCountSections(char v[]) {
  /* Counts the number of sections inside a version string.
     E.g. '1.2.3.4' ==> returns 4                           */

  short n = 1;
  for (short i = 0; i < strlen(v); i++)
  {
    if (v[i] == '.')
    {
      n++;
    }
  }
  return n;
}

unsigned short extractSection(char v[], short v_s, short s) {
  /* Extract the s-th section from a version string v[] with v_s sections. */

  short n = 0;
  short j = 0;
  char buf[6] = {};

  // Take the s section number
  for (short i = 0; i < strlen(v); i++)
  {
    // Copy the current digit to the buffer string
    buf[j] = v[i];
    j++;

    // Check if we are going into a new section
    if (v[i] == '.')
    {
      if (n == s)
      {
        // We just analyzed the s section
        break;
      }
      else
      {
        // Section s not yet reached: go to next one
        n++;
        // ... and restart writing into the buffer string
        j = 0;
      }
    }
  }

  // Terminate the buffer string
  if (s == v_s)
  {
    // The last section does not have a '.' at the end
    buf[j + 1] = '\0';
  } else {
    // The not-last section has a '.' at the end, overwrite it
    buf[j] = '\0';
  }

  return atol(buf);
}
