#include "ini_handler.hxx"

ini_handler::ini_handler(const std::string& filename)
{
    // We initialize our variable.
    this->filename = filename;
    this->error = false;

    // Open our current file

    this->file.open(filename);


    std::string actual_line,actual_section,key,value;

    bool in_section = false;

    // To save our current section.
    std::unordered_map<std::string,std::string> section;

    // Check if the file is good
    if(this->file.is_open())
    {
        // Read everyline
        while(std::getline(file,actual_line))
        {
            // Line is empty? Skip the line.
            if(!actual_line.size())
                continue;

            // Section found
            if(actual_line[0] == '[' && actual_line[actual_line.size() - 1] == ']')
            {
                if(actual_section != this->s_get_section(actual_line))
                {
                    // Not empty section or first section/line
                    if(!section.empty() && !actual_section.empty())
                    {
                        // We add to our file the finished section.
                        config[actual_section] = section;

                        // Clear the current section
                        section.clear();
                    }
                    // Set new section
                    actual_section = this->s_get_section(actual_line);
                }
                in_section = true;
            }
            else
            {
                // In_section
                if(in_section)
                {
                    // Get our line
                    if(this->get_line(actual_line,key,value))
                    {
                        section[key] = value;
                    }
                    else
                    {
                        // This is a commentary? do nothing.
                    }
                }
            }
        }
        // Add our last section
        if(!section.empty())
        {
            config[actual_section] = section;
            section.clear();
        }


        // Don't need the file anymore.
        file.close();
    }
    else
    {
        // Some random error.
        this->error = true;
    }
}

void ini_handler::close()
{
    // Safe check for our current file
    if(filename.size() == 0)
        return;
    // We already have our file in the buffer, so we delete the content of our file.
    this->file.open(filename, std::ofstream::out | std::ofstream::trunc);

    // Here, we set up the content for the ending of file. Plus, handling the last empty line in the file.
    std::size_t count = 0,total_section = config.size(),total_key,key_count = 0;



    //We start to go through every section
    for(auto section: config)
    {
        ++count;
        key_count = 0;

        file << '[' + section.first + ']' << std::endl;
        total_key = section.second.size();

        // We start to go through every key
        for(auto key : section.second)
        {
            ++key_count;
            if(key_count != total_key)
            {
                file << key.first + " = " + key.second << std::endl;
            }
            else
            {
                file << key.first + " = " + key.second;
            }

        }
        if(count != total_section)
        {
            file << std::endl << std::endl;
        }
    }


    // We clear the old content.
    this->config.clear();
    this->filename.clear();
    this->file.close();
    this->error = false;
}


bool ini_handler::error_check()
{
    return this->error;
}

//Set functions

void ini_handler::set(const std::string& section, const std::string& key, const std::string& value)
{
    config[section][key] = value;
}

void ini_handler::set_int(const std::string& section, const std::string& key, const unsigned long long& value)
{
    this->set(section,key, std::to_string(value));
}

void ini_handler::set_real(const std::string& section, const std::string& key, const double& value)
{
    this->set(section,key,std::to_string(value));
}

void ini_handler::set_boolean(const std::string& section, const std::string& key, const bool& value)
{
    if(value)
        this->set(section,key,"True");
    else
        this->set(section,key,"False");
}

// Get functions

std::string ini_handler::get(const std::string& section, const std::string& key)
{
    if(!this->config.count(section) || !this->config[section].count(key))
    {
        return "";
    }
    return this->config[section][key];
}

unsigned long long ini_handler::get_int(const std::string& section, const std::string& key)
{
    std::string::size_type sz {0};
    const std::string str = this->get(section,key);

    return str.size() ? std::stoull(str,&sz,0) : 0;
}

double ini_handler::get_real(const std::string& section, const std::string& key)
{
    std::string::size_type sz {0};
    const std::string str = this->get(section,key);

    return str.size() ? std::stod(str,&sz) : 0;
}

bool ini_handler::get_boolean(const std::string& section, const std::string& key)
{
    const std::string str = this->get(section,key);
    return str == "True" ? true : false;
}

std::set<std::string> ini_handler::get_section() const
{
    std::set<std::string> buffer;
    for(auto section: config)
    {
        buffer.insert(section.first);
    }
    return buffer;
}

std::set<std::string> ini_handler::get_fields(const std::string& section)
{
    std::set<std::string> buffer;

    if(!config.count(section))
        return buffer;


    for(auto key : config[section] )
    {
        buffer.insert(key.first);
    }
    return buffer;
}

//Delete functions

bool ini_handler::del_key(const std::string& section, const std::string& key)
{
    return config[section].erase(key);
}

bool ini_handler::del_section(const std::string& section)
{
    return config.erase(section);
}

//SNIPPETS

inline std::string ini_handler::s_get_section(std::string line) const
{
    line.erase(line.begin());
    line.erase(line.end() - 1);

    return line;
}

bool ini_handler::get_line(const std::string& line, std::string& key, std::string&value)
{
    auto found = line.find('=');
    if(found)
    {
        //this is valid.
        key = line.substr(0,found - 1);
        value = line.substr(found + 2, line.size() - 1);

        return true;
    }

    return false;
}
