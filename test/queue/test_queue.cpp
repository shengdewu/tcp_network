#include "public/ring_queue.h"
#include "public/active_thread.h"
#include <chrono>

struct usr_data
{
    std::string key;
    std::string name;
};

class consumer
{
public:
    consumer():queue_("boofish")
    { 
        for(int i =0; i < 4; ++i)
        {
            input_thread.push_back(std::make_shared<active_thread>(std::bind(&consumer::write_queue, this)));
            output_thread.push_back(std::make_shared<active_thread>(std::bind(&consumer::read_queue, this)));
        }

    }

    ~consumer()
    {
        queue_.drop_out();
        input_thread.clear();
        output_thread.clear();
    }

private:
    inline void read_queue();
    inline void write_queue();

private:
    std::vector<std::shared_ptr<active_thread>> input_thread;
    std::vector<std::shared_ptr<active_thread>> output_thread;
    ring_queue<usr_data> queue_;
};

void consumer::read_queue()
{
    std::shared_ptr<usr_data> data = queue_.pop();

    LOG(LOGI_LVL::LOGI_DEBUG, "data[%s-%s]\n", data->key.c_str(), data->name.c_str()); 
}

void consumer::write_queue()
{
    std::shared_ptr<usr_data> data = std::make_shared<usr_data>();
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(24));
    data->key = std::to_string(now_c);
    data->name = std::to_string(now_c) + "name";

    LOG(LOGI_LVL::LOGI_DEBUG, "push data\n"); 

    queue_.push(data);
}


int main(int argc, char const *argv[])
{
    consumer con;
 
    while(true);

    return 0;
}
