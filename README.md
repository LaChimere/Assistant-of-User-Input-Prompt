* Adopted a case-independent approach, used Trie data structure to create a dictionary based on the frequency of the words’ appearance; used the minimum edit distance algorithm realized by dynamic programming to create an index file to narrow the range of candidate words.
* Used the socket and the thread pool created by the NPTL to build the Server framework; encapsulated the query word sent by the Client into a task and handed it over to the thread pool for processing.
* Created a worker thread in the thread pool and handed the query words sent by the Client to the worker thread; searched for similar candidate words through the query module; eliminated the candidate words failing to meet the requirements of minimum edit distance, and sorted them according to a certain priority (i.e. smaller edit distance, larger word frequency and lexicographic order), selected a specified number of candidate words and return that result to the Client.
* Packaged the data sent to the Client in the JSON format and sent it to the Client.
* Realized the function of sending the Client the prompt candidate words that are the same or similar to the content input by the Client in the JSON format.