#ifndef SERVERCONFIG_H__
#define SERVERCONFIG_H__
#include <iostream>
namespace skynet
{
	class ServerConfig
	{
	public:
		enum Environment
		{
			Dev           =  1,
			Test		  =  2, 
			Sandbox		  =  3,
			Official	  =  4,
			Self		  =  5,
		};
		ServerConfig();
		ServerConfig(Environment type);
		ServerConfig(const ServerConfig & config);
		~ServerConfig();

		const std::string& getServer() const;
		const std::string& getDomain() const;
		const int getPort() const;
		const std::string& getResource() const;
		const std::string& getPassword() const;
		ServerConfig& operator=(const ServerConfig & config);

	private:
		std::string mServer;
		std::string mDomain;
		int mPort;
		std::string mResource;
		std::string mPassword;
	private:
		static const std::string DEV_HOST;
		static const std::string DEV_DOMAIN;
		static const int DEV_TCPPORT;
		static const std::string DEV_RESOURCE;
		static const std::string DEV_PASSWORD;

		static const std::string OFFICAL_HOST;
		static const std::string OFFICIAL_DOMAIN;
		static const int OFFICAL_TCPPORT;
		static const std::string OFFICAL_RESOURCE;
		static const std::string OFFICAL_PASSWORD;

		static const std::string SELF_HOST;
		static const std::string SELF_DOMAIN;
		static const int SELF_TCPPORT;
		static const std::string SELF_RESOURCE;
		static const std::string SELF_PASSWORD;
	};
}


#endif //SERVERCONFIG_H__