#pragma once
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using std::string;
class HttpClient
{
public:
	static int post(const string& host, const string& port, const string& page, const string& data, string& reponse_data)
	{
		try
		{
			boost::asio::io_service io_service;
			//���io_service���ڸ��õ����
			if (io_service.stopped())
				io_service.reset();

			// ��dnsȡ�������µ�����ip
			tcp::resolver resolver(io_service);
			tcp::resolver::query query(host, port);
			tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

			// �������ӵ����е�ĳ��ipֱ���ɹ� 
			tcp::socket socket(io_service);
			boost::asio::connect(socket, endpoint_iterator);

			// Form the request. We specify the "Connection: close" header so that the
			// server will close the socket after transmitting the response. This will
			// allow us to treat all data up until the EOF as the content.
			boost::asio::streambuf request;
			std::ostream request_stream(&request);
			request_stream << "POST " << page << " HTTP/1.0\r\n";
			request_stream << "Host: " << host << ":" << port << "\r\n";
			request_stream << "Accept: */*\r\n";
			request_stream << "Content-Length: " << data.length() << "\r\n";
			request_stream << "Authorization: Bearer 9SWLUIBl9sDF7b3JkMapXtfj41kZ2WJQ\r\n";
			request_stream << "Content-Type: application/json\r\n";
			request_stream << "Connection: close\r\n\r\n";
			request_stream << data;

			// Send the request.
			boost::asio::write(socket, request);

			// Read the response status line. The response streambuf will automatically
			// grow to accommodate the entire line. The growth may be limited by passing
			// a maximum size to the streambuf constructor.
			boost::asio::streambuf response;
			boost::asio::read_until(socket, response, "\r\n");

			// Check that response is OK.
			std::istream response_stream(&response);
			std::string http_version;
			response_stream >> http_version;
			unsigned int status_code;
			response_stream >> status_code;
			std::string status_message;
			std::getline(response_stream, status_message);
			if (!response_stream || http_version.substr(0, 5) != "HTTP/")
			{
				reponse_data = "Invalid response";
				return -2;
			}
			// ������������ط�200����Ϊ�д�,��֧��301/302����ת
			if (status_code != 200)
			{
				reponse_data = "Response returned with status code != 200 ";
				return status_code;
			}

			// ��˵�еİ�ͷ���Զ�������
			std::string header;
			std::vector<string> headers;
			while (std::getline(response_stream, header) && header != "\r")
				headers.push_back(header);

			// ��ȡ����ʣ�µ�������Ϊ����
			boost::system::error_code error;
			while (boost::asio::read(socket, response,
				boost::asio::transfer_at_least(1), error))
			{
			}

			//��Ӧ������
			if (response.size())
			{
				std::istream response_stream(&response);
				std::istreambuf_iterator<char> eos;
				reponse_data = string(std::istreambuf_iterator<char>(response_stream), eos);
			}

			if (error != boost::asio::error::eof)
			{
				reponse_data = error.message();
				return -3;
			}
		}
		catch (std::exception& e)
		{
			reponse_data = e.what();
			return -4;
		}
		return 0;
	}
};