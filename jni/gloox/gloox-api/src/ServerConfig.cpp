#include "ServerConfig.h"
namespace skynet
{
	const std::string ServerConfig::DEV_HOST("192.168.0.63");
	const std::string ServerConfig::DEV_DOMAIN("johnny-pc");
	const int ServerConfig::DEV_TCPPORT = 5222;
	const std::string ServerConfig::DEV_RESOURCE("jw");
	const std::string ServerConfig::DEV_PASSWORD("idswz");

	const std::string ServerConfig::OFFICAL_HOST("cac.msg.iplaychess.cn");
	const std::string ServerConfig::OFFICIAL_DOMAIN("gz-qp-web1");
	const int ServerConfig::OFFICAL_TCPPORT = 5222;
	const std::string ServerConfig::OFFICAL_RESOURCE("qp");
	const std::string ServerConfig::OFFICAL_PASSWORD("idswz");

	const std::string ServerConfig::SELF_HOST("192.168.112.68");
	const std::string ServerConfig::SELF_DOMAIN("nick-pc");
	const int ServerConfig::SELF_TCPPORT = 5222;
	const std::string ServerConfig::SELF_RESOURCE("nw");
	const std::string ServerConfig::SELF_PASSWORD("123456");

	ServerConfig::ServerConfig()
	{

	}

	ServerConfig::ServerConfig(Environment type)
	{
		if(type == Environment::Dev)
		{
			mServer = DEV_HOST;
			mDomain = DEV_DOMAIN;
			mPort = DEV_TCPPORT;
			mResource = DEV_RESOURCE;
			mPassword = DEV_PASSWORD;
		}else if(type == Environment::Test)
		{
			mServer = DEV_HOST;
			mDomain = DEV_DOMAIN;
			mPort = DEV_TCPPORT;
			mResource = DEV_RESOURCE;
			mPassword = DEV_PASSWORD;
		}else if(type == Environment::Sandbox)
		{
			mServer = DEV_HOST;
			mDomain = DEV_DOMAIN;
			mPort = DEV_TCPPORT;
			mResource = DEV_RESOURCE;
			mPassword = DEV_PASSWORD;
		}else if(type == Environment::Self)
		{
			mServer = SELF_HOST;
			mDomain = SELF_DOMAIN;
			mPort = SELF_TCPPORT;
			mResource = SELF_RESOURCE;
			mPassword = SELF_PASSWORD;
		}
		else 
		{
			mServer = OFFICAL_HOST;
			mDomain = OFFICIAL_DOMAIN;
			mPort = OFFICAL_TCPPORT;
			mResource = OFFICAL_RESOURCE;
			mPassword = OFFICAL_PASSWORD;
		}
	}

	ServerConfig::ServerConfig(const ServerConfig & config)
	{
		*this = config;
	}

	ServerConfig::~ServerConfig()
	{
	}

	const std::string& ServerConfig::getServer() const
	{
		return mServer;
	}

	const std::string& ServerConfig::getDomain() const
	{
		return mDomain;
	}
	const int ServerConfig::getPort() const
	{
		return mPort;
	}
	const std::string& ServerConfig::getResource() const
	{
		return mResource;
	}

	const std::string& ServerConfig::getPassword() const
	{
		return mPassword;
	}

	ServerConfig& ServerConfig::operator=(const ServerConfig & config)
	{
		this->mServer = config.mServer;
		this->mDomain = config.mDomain;
		this->mPort = config.mPort;
		this->mResource = config.mResource;
		this->mPassword = config.mPassword;
		return *this;
	}
}
