#include <eosiolib/eosio.hpp>
#include "delfos.hpp"

using namespace eosio;

void delfos::store( std::string url, std::string memo ) {
  eosio_assert( url.size() <= 256, "url has more than 256 bytes" );
  eosio_assert( memo.size() <= 1024, "memo has more than 1024 bytes" );
  require_auth( _self );

  save_memo(url, memo);
}

void delfos::save_memo(std::string url, std::string memo) {
  ds_memos memos(_self, _self);
  memos.emplace( _self, [&]( auto& m ) {
    m.key = memos.available_primary_key();
    m.url = url;
    m.hash = delfos::hash_string(url);
    m.memo = memo;
    m.created_at = (uint64_t) now();
  });
}

EOSIO_ABI( delfos, (store) )
