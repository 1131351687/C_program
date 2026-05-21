#include "client.h"

Client::Client(std::string id, const Server& server)
    : id(id), server(&server)
{
    crypto::generate_key(public_key, private_key);//生成公私钥对
}
std::string Client::get_id()
{
    return id;
}
std::string Client::get_publickey() const
{
    return public_key;
}
double Client::get_wallet()
{
    return server->get_wallet(id);
}
std::string Client::sign(std::string txt) const
{
    return crypto::signMessage(private_key, txt);
}
bool Client::transfer_money(std::string receiver, double value)
{
    string trx=get_id()+"-"+receiver+"-"+to_string(value);
    string sig=sign(trx);
    return server->add_pending_trx(trx, sig);
}
size_t Client::generate_nonce()
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<size_t> distribution(0, 999999999);
    return distribution(generator);
}