#include "server.h"
#include "client.h"

vector<std::string> pending_trxs;

Server::Server()
{
}

shared_ptr<Client> Server::add_client(string id)
{
    bool id_exists = false;
    for (const auto &pair : clients)
    {
        if (pair.first->get_id() == id)
        {
            id_exists = true;
            break;
        }
    }
    if (!id_exists)
    {
        auto new_client = make_shared<Client>(id, *this);
        clients[new_client] = 5.0;
        return new_client;
    }
    else
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dist(1000, 9999);

        string final_id;
        do
        {
            int suffix = dist(gen);
            final_id = id + std::to_string(suffix);

            // 检查新 id 是否重复
            bool id_used = false;
            for (const auto &pair : clients)
            {
                if (pair.first->get_id() == final_id)
                {
                    id_used = true;
                    break;
                }
            }

            if (!id_used)
            {
                break;
            }
        } while (true);

        auto new_client = make_shared<Client>(final_id, *this);
        clients[new_client] = 5.0;
        return new_client;
    }
}

shared_ptr<Client> Server::get_client(std::string id) const
{
    for (const auto &pair : clients)
    {
        if (pair.first->get_id() == id)
        {
            return pair.first;
        }
    }
    return nullptr; // 返回空指针表示未找到客户端
}

double Server::get_wallet(std::string id) const
{
    for (const auto &pair : clients)
    {
        if (pair.first->get_id() == id)
        {
            return pair.second;
        }
    }
    return -1; // 返回 -1 表示未找到客户端
}

bool Server::parse_trx(string trx, string &sender, string &receiver, double &value)
{
    size_t p1 = trx.find('-');
    size_t p2 = trx.find('-', p1 + 1);
    if (p1 == string::npos || p2 == string::npos)
    {
        throw runtime_error("invalid transaction format");
    }
    // 解析发送者,substr(pos,len)从pos位置开始，截取len长度的字符串
    sender = trx.substr(0, p1);
    receiver = trx.substr(p1 + 1, p2 - p1 - 1);
    string value_str = trx.substr(p2 + 1);
    if (sender.empty() || receiver.empty() || value_str.empty())
    {
        throw runtime_error("invalid transaction format");
    }
    try
    {
        // 将字符串转换为 double 类型
        // stod()函数是 C++11 引入的标准库函数，用于将字符串转换为 double 类型
        value = stod(value_str);
    }
    catch (...)
    {
        throw runtime_error("invalid transaction format");
    }
    return true;
}

bool Server::add_pending_trx(std::string trx, std::string signature) const
{

    /**
     * 验证交易的有效性并将其加入待处理列表。
     * 该函数执行以下检查：
     * 1. 解析交易数据，若格式错误则拒绝。
     * 2. 验证发送者是否存在且拥有有效的公钥。
     * 3. 使用发送者的公钥验证交易签名的真实性。
     * 4. 检查发送者钱包余额是否充足。
     * @param trx 交易字符串，包含发送者、接收者和金额信息。
     * @param signature 交易的数字签名，用于验证身份和完整性。
     * @return true 如果交易通过所有验证并成功加入待处理列表；否则返回 false。
     */
    string sender, receiver;
    double value;
    // 解析交易数据，若解析失败则直接返回 false
    try
    {
        parse_trx(trx, sender, receiver, value);
    }
    catch (...)
    {
        return false;
    }
    // 获取发送者客户端对象，若不存在则拒绝交易
    auto sender_client = get_client(sender);
    if (sender_client == nullptr)
        return false;
    auto receiver_client = get_client(receiver);
    if (receiver_client == nullptr)
        return false;
    string public_key = sender_client->get_publickey();
    // 验证交易签名，确保交易由发送者授权且未被篡改
    if (!crypto::verifySignature(public_key, trx, signature))
        return false;

    // 检查发送者钱包余额是否足以支付交易金额
    if (get_wallet(sender) < value)
        return false;

    // 将验证通过的交易加入待处理列表
    pending_trxs.push_back(trx);
    return true;
}

size_t Server::mine()
{
    // 检查待处理交易列表是否为空，若为空则直接返回0
    if (pending_trxs.empty())
    {
        return 0;
    }
    // 将所有待处理交易拼接成一个字符串，用于后续哈希计算
    string sha256;
    for (auto trx : pending_trxs)
    {
        sha256 += trx;
    }
    // 遍历所有客户端，尝试生成符合难度要求的nonce
    for (const auto &pair : clients)
    {
        shared_ptr<Client> client = pair.first;

        for (size_t attempt = 0; attempt < 100000; ++attempt)
        {
            size_t nonce = client->generate_nonce();
            string hash_result = crypto::sha256(sha256 + to_string(nonce));
            if (hash_result.substr(0, 10).find("000") != string::npos)
            {
                cout << "Miner is "
                     << client->get_id()
                     << endl;
                clients[client] += 6.25;

                for (const auto &trx : pending_trxs)
                {
                    string sender, receiver;
                    double value;
                    parse_trx(trx, sender, receiver, value);

                    auto sender_ptr = get_client(sender);
                    auto receiver_ptr = get_client(receiver);

                    if (sender_ptr && receiver_ptr)
                    {
                        clients[receiver_ptr] += value;
                        clients[sender_ptr] -= value;
                    }
                }
                pending_trxs.clear();
                return nonce;
            }
        }
    }
}
