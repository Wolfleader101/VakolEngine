#pragma once

#include <string>
#include <memory>

namespace Vakol::Controller
{
	class Serializable //Type Erasure
	{
       public:

		template<class T>
        Serializable(T&& type) { *this = std::forward<T>(type);}

		template<class T>
		Serializable& operator=(T&& type)
		{
			//decay the type 
            value.reset(new StoredValue<std::decay_t<T>>(std::forward<T>(type)));
			return *this;
		}

        void Serialize(const std::string &file) const;
        void Deserialize(const std::string &file);

	   private:

		   struct SerializableType //base for actual storage
		   {
                virtual ~SerializableType(){};

                virtual void Serialize(const std::string& file) const = 0;
                virtual void Deserialize(const std::string& file) = 0;
		   };

		   template <class T>
		   struct StoredValue : SerializableType //type to actually store T
		   {
                StoredValue(T intake) : actualStorage(intake) {}
				StoredValue(T&& intake) : actualStorage(std::move(intake)) {}

				void Serialize(const std::string& file) const override { actualStorage.Serialize(file); }
				void Deserialize(const std::string& file) override { actualStorage.Deserialize(file); }

                T& actualStorage;
		   };

			std::unique_ptr<SerializableType> value;
	};
}