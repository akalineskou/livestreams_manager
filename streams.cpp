#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>

#ifdef _WIN32
    #include <conio.h>
#endif

using namespace std;

void system_pause()
{
    #ifdef _WIN32
        system("PAUSE");
    #else
        system("read -p \"Press any key to continue...\"");
    #endif
}

void system_clear()
{
    #ifdef _WIN32
        system("CLS");
    #else
        system("clear");
    #endif
}

void print_spaces(int num)
{
    int i;

    for (i = 0; i < num; i++)
        cout << " ";
}

vector< vector<string> > getFileContents(string file_name)
{
    ifstream stream_file(file_name.c_str());
    if (stream_file.is_open())
    {
        stream_file.seekg(0, ios::end);
        int empty = stream_file.tellg();
        stream_file.seekg(0, ios::beg);

        if (empty != 0)
        {
            string name, url, quality, data;
            vector< vector<string> > lines;

            while (getline(stream_file, data))
            {
                vector<string> line;

                if (data.empty())
                {
                    line.push_back("");
                    line.push_back("");
                    line.push_back("");
                }
                else
                {
                    stringstream iss(data, istringstream::in);
                    iss >> name >> url >> quality;

                    line.push_back(name);
                    line.push_back(url);
                    line.push_back(quality);

                    url = quality = "";
                }

                lines.push_back(line);
            }

            stream_file.close();

            return lines;
        }
        else
        {
            cout << "No live streams were found, file \"" << file_name << "\" is empty!" << endl << endl;

            stream_file.close();

            system_pause();
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        cout << "Unable to open file \"" << file_name << "\" to read!" << endl << endl;

        system_pause();
        exit(EXIT_FAILURE);
    }
}

string getFileName(int argc, char *argv[])
{
    string file_name;
    if (argc != 2)
    {
        ifstream stream_file("streams.txt");
        if (!stream_file.is_open())
        {
            cout << "The live stream file was not correctly passed through the command line." << endl;
            cout << "File \"streams.txt\" was also not found in the program's folder." << endl << endl;

            stream_file.close();

            system_pause();
            exit(EXIT_FAILURE);
        }
        else
        {
            stream_file.close();

            return "streams.txt";
        }
    }

    return argv[1];
}

int main(int argc, char *argv[])
{
    string file_name = getFileName(argc, argv), command_str, stream_id_string, quality;
    vector< vector<string> > streams = getFileContents(file_name), streams_temp;
    stringstream command;
    int stream_id, i, c, c2, temp_c, space_length = 17;

    for (i = 0; i < (int)streams.size(); i++)
    {
        if (!streams[i][0].empty() && !streams[i][1].empty() && !streams[i][2].empty())
        {
            vector<string> line_temp;

            line_temp.push_back(streams[i][0]);
            line_temp.push_back(streams[i][1]);
            line_temp.push_back(streams[i][2]);

            streams_temp.push_back(line_temp);
        }
    }

    do
    {
        cout << "Available live streams:" << endl << endl;

        c = 1;
        int double_column = (streams.size() > 21 ? 1 : 0);

        if (double_column)
        {
            c2 = ceil((float)streams.size() / 2);
            temp_c = c2;

            for (i = 0; i < c2; i++)
                if (((!streams[i][0].empty() && (streams[i][1].empty() || streams[i][2].empty())) || !(!streams[i][0].empty() && !streams[i][1].empty() && !streams[i][2].empty())))
                    temp_c--;
        }

        for (i = 0; (unsigned int)i < (double_column ? c2 : streams.size()); i++)
        {
            if (!streams[i][0].empty() && (streams[i][1].empty() || streams[i][2].empty()))
            {
                cout << "-" << streams[i][0];

                (double_column ? print_spaces(space_length - (1 + streams[i][0].length())) : (void)printf("\n"));
            }
            else if (!streams[i][0].empty() && !streams[i][1].empty() && !streams[i][2].empty())
            {
                print_spaces(c < 10 ? 4 : 3);
                cout << c << ": " << streams[i][0] << "(" << streams[i][2] << ")";

                (double_column ? print_spaces(space_length - ((c < 10 ? 4 : 3) + sizeof(c) + (streams[i][0].length()))) : (void)printf("\n"));

                c++;
            }
            else
                (double_column ? print_spaces(space_length) : (void)printf("\n"));

            if (double_column)
            {
                if (c2 + (unsigned int)i == streams.size())
                    break;

                if (!streams[c2 + i][0].empty() && (streams[c2 + i][1].empty() || streams[c2 + i][2].empty()))
                    cout << "\t\t\t-" << streams[c2 + i][0] << endl;
                else if (!streams[c2 + i][0].empty() && !streams[c2 + i][1].empty() && !streams[c2 + i][2].empty())
                {
                    cout << "\t\t\t   " << temp_c + 1 << ": " << streams[c2 + i][0] << "(" << streams[c2 + i][2] << ")" << endl;

                    temp_c++;
                }
                else
                    cout << endl;
            }
        }

        if (double_column && (streams.size() % 2 != 0))
            cout << endl;

        cout << endl << "Live stream number(M: Manual Live Stream | R: Reload Streams | X: Exit)";
        cout << endl << "Input stream quality after number to overwrite default quality!" << endl;
        getline(cin, stream_id_string);

        stringstream iss(stream_id_string, istringstream::in);
        iss >> stream_id >> quality;

        if (atoi(stream_id_string.c_str()) == stream_id && (stream_id > 0 && stream_id <= (int)streams_temp.size()))
        {
            #ifdef _WIN32
            char choice;

            do
            {
            #endif
                system_clear();

				quality = (quality.length() > 0 ? quality : streams_temp[stream_id-1][2]);

                cout << "Starting live stream: " << streams_temp[stream_id-1][0] << "(" << streams_temp[stream_id-1][1] << " " << quality << ")" << endl << endl;

                command.str("");
                command << "livestreamer " << streams_temp[stream_id-1][1] << " " << quality;
                command_str = command.str();

				quality = "";

                system(command_str.c_str());

            #ifdef _WIN32
                cout << endl << "Restart last stream(y/Y)? Or press any other key to go back.";
                choice = _getch();

                system_clear();
            } while (choice == 'y' || choice == 'Y');
            #else
            system_pause();
            system_clear();
            #endif

        }
        else if (stream_id_string == "m" || stream_id_string == "M")
        {
            system_clear();

            string url_quality, temp[3], s;

            cout << "Insert live stream URL and/or quality seperated by space:" << endl;
            cout << "(\"stream_name\" at the end seperated by space to save it to the file)" << endl;
            getline(cin, url_quality);

            temp[2] = temp[1] = "-1";
            stringstream temp_string(url_quality);
            int i = 0;
            while ( getline( temp_string, s, ' ' ) ) {
                temp[i] = s.c_str();

                i++;

                if (i > 2) break;
            }

            // default quality High
            temp[1] = (temp[1] == "-1" ? "High" : temp[1]);

            if (temp[2] != "-1")
            {
                ofstream myfile;
                myfile.open(file_name.c_str(), ios_base::app);
                myfile << endl << endl << temp[2] << " " << temp[0] << " " << temp[1];
                myfile.close();
            }

            //refresh file contents after saving
            streams = getFileContents(file_name);

            for (i = 0; i < (int)streams.size(); i++)
            {
                if (!streams[i][0].empty() && !streams[i][1].empty() && !streams[i][2].empty())
                {
                    vector<string> line_temp;

                    line_temp.push_back(streams[i][0]);
                    line_temp.push_back(streams[i][1]);
                    line_temp.push_back(streams[i][2]);

                    streams_temp.push_back(line_temp);
                }
            }

            #ifdef _WIN32
            char choice;

            do
            {
            #endif
                system_clear();

                cout << "Starting manual live stream: " << temp[0] << " " << temp[1] << endl;

                command.str("");
                command << "livestreamer " << temp[0] << " " << temp[1];
                command_str = command.str();

                cout << endl;

                system(command_str.c_str());

            #ifdef _WIN32
                cout << endl << "Restart last stream(y/Y)? Or press any other key to go back.";
                choice = _getch();

                system_clear();
            } while (choice == 'y' || choice == 'Y');
            #else
            system_pause();
            system_clear();
            #endif
        }
        else if (stream_id_string == "r" || stream_id_string == "R")
        {
            //refresh file contents after saving
            streams = getFileContents(file_name);

            for (i = 0; i < (int)streams.size(); i++)
            {
                if (!streams[i][0].empty() && !streams[i][1].empty() && !streams[i][2].empty())
                {
                    vector<string> line_temp;

                    line_temp.push_back(streams[i][0]);
                    line_temp.push_back(streams[i][1]);
                    line_temp.push_back(streams[i][2]);

                    streams_temp.push_back(line_temp);
                }
            }

            system_clear();
        }
        else
        {
            if (stream_id_string != "x" && stream_id_string != "X")
            {
                system_clear();

                cout << "Invalid live stream number!" << endl << endl;

                system_pause();
                system_clear();
            }
        }

    } while (stream_id_string != "x" && stream_id_string != "X");

    system_clear();

    cout << "\t\t\t\tGoodbye!!!";

    return EXIT_SUCCESS;
}