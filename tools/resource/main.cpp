#include <fstream>
#include <system_error>
#include <cxxopts.hpp>

struct Config
{
    std::vector<std::string> input;
    std::string output;
};

struct ResourceFile
{
    static std::size_t constexpr byte_wrap = 16U;

    ResourceFile(std::string const &path)
    {}

    bool process(std::string const &input, std::string const &output, std::error_code &ec)
    {
        bool rv = false;

        std::ifstream i(input);
        output_.open(output, std::ios_base::out);
    
        if(i.good())
        {
            char buffer[1024];
    
            while(!i.eof())
            {
                std::size_t count = i.readsome(buffer, sizeof(buffer));
    
                writeBody(reinterpret_cast<uint8_t *>(buffer), count, ec);
            }
        }
    
        return rv;
    }

    bool writeBody(uint8_t const *data, std::size_t count, std::error_code __attribute__((unused)) &ec)
    {
        std::size_t nrows = count / byte_wrap;

        output_ << std::setw(2) << std::setfill('0');

        for(std::size_t rowIndex = 0; rowIndex < nrows; ++rowIndex)
        {
            for(std::size_t columnIndex = 0; columnIndex < byte_wrap; ++columnIndex)
            {
                std::size_t offset = (rowIndex * byte_wrap) + columnIndex;

                output_ << std::hex << data[offset];
            }

            output_ << std::endl;
        }

        return true;
    }

    bool writeHeader(std::error_code &)
    {
        std::string thing = "STUPID_GUARD_H_";

        output_ << "#ifndef " << thing << std::endl;
        output_ << "#define " << thing << std::endl;
        output_ << std::endl;
        output_ << "static uint8_t const bytes[] =" << std::endl;
        output_ << "{" << std::endl;

        return true;
    }

    bool writeTrailer(std::error_code &)
    {
        output_ << "}" << std::endl;
        output_ << "#endif" << std::endl;

        return true;
    }

    std::ofstream output_;
};

class ResourceWriter
{
public:
    ResourceWriter(std::string const &directory);

private:
    std::string directory_;
}

static bool processFile(std::string const &input, std::string const &output, std::error_code &ec)
{
    bool rv = false;

    ResourceFile resource("");
    resource.process(input, output, ec);

    return rv;
}

static bool parseCommandLine(Config &config, int argc, char const *argv[], std::error_code &ec)
{
    cxxopts::Options options("", "");
    bool rv = false;

    options.add_options("xxx")
        ("i,input", "", cxxopts::value(config.input), "")
        ("o,output", "", cxxopts::value(config.output), "")
        ;

    try
    {
        auto res = options.parse(argc, argv);
        rv = true;
    }
    catch(const std::exception& e)
    {
        ec = std::make_error_code(std::errc::invalid_argument);
    }

    return rv;
}

int main(int argc, char const *argv[])
{
    std::error_code ec;
    Config config;

    if(parseCommandLine(config, argc, argv, ec))
    {
        for(auto const &input : config.input)
        {
            processFile(input, config.output, ec);
        }
    }

    return 0;
}