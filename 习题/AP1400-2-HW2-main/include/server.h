#ifndef SERVER_H
#define SERVER_H
#include <map>
#include <memory>
#include "crypto.h"
#include <string>
#include <random>
#include <stdexcept> // 添加头文件以支持 std::runtime_error
#include <vector>

using namespace std;

class Client;
extern std::vector<std::string> pending_trxs;

class Server
{
public:
	Server();
	// 添加一个新的客户端，如果 id 已存在，则在 id 后添加一个随机的四位数后缀，直到生成一个唯一的 id
	std::shared_ptr<Client> add_client(std::string id);

	// 添加一个待处理的交易，返回是否添加成功
	std::shared_ptr<Client> get_client(std::string id) const;

	// 获取指定 id 的客户端的余额
	double get_wallet(std::string id) const;

	// 解析交易，并输出交易的发送方、接收方和金额，返回解析是否成功
	static bool parse_trx(std::string trx, std::string& sender, std::string& receiver, double& value);

	// 添加一个待处理的交易，返回是否添加成功
	bool add_pending_trx(std::string trx, std::string signature) const;

	// 挖矿函数，为所有待处理的交易生成一个nonce，并将成功挖矿的客户端奖励 6.25 个货币，返回成功挖矿的交易数量
	size_t mine();

	friend void show_wallets(const Server& server);
private:
	std::map<std::shared_ptr<Client>, double> clients;
};


#endif //SERVER_H