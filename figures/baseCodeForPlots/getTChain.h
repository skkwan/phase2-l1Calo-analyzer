#ifndef GET_TCHAIN_H_INCL
#define GET_TCHAIN_H_INCL

/*
 * Returns TChain of the specified tree, from a single file at rootFileDirectory.
 */
TChain* getTChainFromSingleFile(TString rootFileDirectory, TString treeName) {

  TChain *ch = new TChain(treeName);

  int result = ch->Add(rootFileDirectory.Data());
  if (result == 0) {
    std::cout << "[ERROR:] getTChain.h: Failed to find " << treeName << "in " << rootFileDirectory << ", exiting " << std::endl;
    return NULL;
  }
  return ch;
}

/*
 * Returns TChain containing the specified tree using an input text file with file names separated by new lines. If startLine and nLinesToRead are specified, read the specified block from the file.
 */
TChain* getTChainFromListOfFiles(TString inputListOfFiles, TString treeName, int startLine = -1, int nLinesToRead = -1) {

    TChain *ch = new TChain(treeName);

    ifstream file;
    std::cout << "Attempting to open input list of files: " << inputListOfFiles << std::endl;
    file.open(inputListOfFiles.Data(), ifstream::in);
    std::string line;
    std::cout << "Input " << inputListOfFiles << " opened successfully" << std::endl;

    if (startLine == -1 || nLinesToRead == -1) {
      std::cout << "Default behavior: Will read all lines in file" << std::endl;
      for (; std::getline(file, line); ) {
        const char *filename = line.c_str();
        int result = ch->Add(filename); 
        if (result == 0) {
          std::cout << "[ERROR:] getTChain.h: Failed to find " << treeName << "in " << inputListOfFiles << ", exiting " << std::endl;
          return NULL;
        }
        std::cout << "... Added " << treeName << " from " << filename << std::endl; 
      }
    }
    else {
      // Only read lines (startLine, startLine + nLinesToRead)
      // Iterate std::getline past the first nth lines
      for (int i = 1; i < startLine; i++ )  {
        std::getline(file, line);
        std::cout << "[~~] Skipping a line" << std::endl;
      }

      // The nth time std::getline is called, it will be on line n
      for (int i = 0; i < nLinesToRead; i++ ) {
	
        std::getline(file, line);
        const char *filename = line.c_str();
        int result = ch->Add(filename);
        if (result == 0) {
          std::cout << "[ERROR:] getTChain.h: Failed to access the file: check that startLine " << startLine 
                    << " and nLinesToRead " << nLinesToRead 
                    << " are within bounds, and that the tree " << treeName << "exists. Exiting..." << std::endl;
          return NULL;
        }

    	  std::cout << "Added: " << filename << endl;
      }
      

    }

    return ch;
}




#endif