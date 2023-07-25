#ifndef INCLUDED_MY_PROTOCONTEXT_HPP
#define INCLUDED_MY_PROTOCONTEXT_HPP

#include <memory>

namespace my
{
	class ProtoSharedContext;
	class ProtoClient;
	class ProtoDataCache;
	class ProtoContext
	{
	private:
		std::shared_ptr<ProtoSharedContext> shared_ctx_;
		ProtoDataCache* mapcc_;

	public:
		ProtoContext();
		~ProtoContext();

		friend ProtoClient;
	};
}

#endif // INCLUDED_MY_PROTOCONTEXT_HPP
