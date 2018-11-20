#include <eosiolib/eosio.hpp>
#include <eosiolib/crypto.h>

using namespace eosio;

class delfos : public eosio::contract {
  public:
    delfos(account_name self) : eosio::contract(self) {}

    //@abi action
    void store( std::string url, std::string memo );

  private:

    void save_memo(std::string url, std::string memo);

    const static checksum256 hash_string(std::string plaintext) {
        checksum256 hash;
        sha256( plaintext.c_str(), plaintext.size(), &hash );
        return hash;
    }

    //@abi table memos i64
    struct oracle_memo {
      uint64_t key;
      std::string url;
      checksum256 hash;
      std::string memo;
      uint64_t created_at;

      auto primary_key() const { return key; }
      key256 get_hash() const {
        const uint64_t *p64 = reinterpret_cast<const uint64_t *>(&hash);
        return key256::make_from_word_sequence<uint64_t>(p64[0], p64[1], p64[2], p64[3]);
      }
      std::string get_url() const { return url; }
      std::string get_memo() const { return memo; }
      uint64_t get_created_at() const { return created_at; }

      EOSLIB_SERIALIZE( oracle_memo, ( key )( url )( hash )( memo )( created_at ) )
    };

    typedef eosio::multi_index<N(memos), oracle_memo,
      indexed_by<N(byhash), const_mem_fun<oracle_memo, key256, &oracle_memo::get_hash>>,
      indexed_by<N(bytime), const_mem_fun<oracle_memo, uint64_t, &oracle_memo::get_created_at>>
    > ds_memos;
};
