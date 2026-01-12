#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>

using timestamp = std::chrono::sys_seconds;

constexpr double SF = 1000;

enum class accountType{
	Liability,
	Asset
};

enum class entryDirection{
	Credit,
	Debit
};

enum class entryStatus{
	Posted,
	Voided
};


struct account{
	std::string name;
	int id;
	accountType type;
	timestamp created_at;
};

struct category{
	int id;
	int account_id;
	std::string name;
	bool is_active;
};

struct ledgerEntry{
	int id;
	int account_id;
	int category_id;
	int64_t amount;
	entryDirection direction;
	timestamp time_stamp;
	std::string description;
	entryStatus status;
};

class Ledger{
	public:
	 double balance(
	  const account& Account,
	  const std::vector<ledgerEntry>& entries,
          const timestamp& upToTimeStamp
	 )const;

	 double total_credits(
	  int accountID,
	  const std::vector<ledgerEntry>& entries,
	  const timestamp& start,
	  const timestamp& end
	 )const;

	 double total_debits(
	  int accountID,
	  const std::vector<ledgerEntry>& entries,
	  const timestamp& start,
	  const timestamp& end
	 )const;

	 double total_category(
	  int accountID,
	  int categoryID,
	  entryDirection entry_direction,
	  const std::vector<ledgerEntry>& entries,
	  const timestamp& start,
	  const timestamp& end
	 )const;
};


constexpr int sign(accountType acc, entryDirection dir) noexcept{
	switch (acc){
		case accountType::Asset:
			return (dir == entryDirection::Debit) ? +1 : -1;
		case accountType::Liability:
			return (dir == entryDirection::Credit) ? +1 : -1; //had a little problem here LOL
	}
}
double Ledger::balance(
		const account& Account,
		const std::vector<ledgerEntry>& entries,
		const timestamp& upToTimeStamp
		)
	const{
		int64_t Balance = 0;
	for (const auto&e : entries){
		if (e.account_id != Account.id) continue;
		if (e.status != entryStatus::Posted) continue;
		if (e.time_stamp > upToTimeStamp) continue;
		
		Balance += e.amount * sign(Account.type, e.direction);
	}
           return static_cast<double>(Balance)/SF;
	}

double Ledger::total_credits(
		int accountID,
		const std::vector<ledgerEntry>& entries,
		const timestamp& start,
		const timestamp& end
		)
const{int64_t total = 0;
	for (const auto&e : entries){
		if (e.account_id != accountID) continue;
		if (e.status != entryStatus::Posted) continue;
		if (e.direction != entryDirection::Credit) continue;
		if (e.time_stamp < start || e.time_stamp > end) continue;

		total += e.amount;
	}
	return static_cast<double>(total)/SF;
}

double Ledger::total_debits(
		int accountID,
		const std::vector<ledgerEntry>& entries,
		const timestamp& start,
		const timestamp& end
		)

	const{int64_t total = 0;
		for (const auto&e : entries){
			if (e.account_id != accountID) continue;
			if (e.status != entryStatus::Posted) continue;
			if (e.direction != entryDirection::Debit) continue;
			if (e.time_stamp < start || e.time_stamp > end) continue;

			total += e.amount;

		}
		return static_cast<double>(total)/SF;
	}
double Ledger::total_category(
		int accountID,
		int categoryID,
		entryDirection entry_direction,
		const std::vector<ledgerEntry>& entries,
		const timestamp& start,
		const timestamp& end
		)
	const{int64_t total = 0;

		for (const auto&e : entries){
			if (e.account_id != accountID) continue;
			if (e.category_id != categoryID) continue;
			if (e.direction != entry_direction) continue;
			if (e.status != entryStatus::Posted) continue;
			if (e.time_stamp < start || e.time_stamp > end) continue;

			total += e.amount;
		}
		return static_cast<double>(total)/SF;
	}

timestamp iso8601_parse(const std::string& s){
	std::istringstream in{s};
	timestamp tp;

	in >> std::chrono::parse("%FT%T%z", tp);
	return tp;

}

int main(){

	Ledger ledger;

	timestamp start = iso8601_parse("2025-01-01T00:00:00Z");
	timestamp end = iso8601_parse("2025-12-31T23:59:59Z");
	account account1{"account1", 1, accountType::Liability, iso8601_parse("2025-01-03T23:21:23Z")};
	category net{1, 1, "net amount", true};
	category away{2, 1, "away from net", true};
	std::vector<ledgerEntry> entries = {
		{1, account1.id, net.id, static_cast<int64_t>(500.234 * SF), entryDirection::Credit, iso8601_parse("2025-06-21T11:23:23Z"), "salary", entryStatus::Posted},
		{2, account1.id, away.id, static_cast<int64_t>(210.221 * SF), entryDirection::Credit, iso8601_parse("2025-07-23T22:12:31Z"), "expenses", entryStatus::Posted}
	};

	std::cout<<ledger.balance(account1, entries, iso8601_parse("2025-08-23T22:21:34Z")) << "\n";
	std::cout<<ledger.total_credits(account1.id, entries, start, end) << "\n";
	std::cout<<ledger.total_debits(account1.id, entries, start, end) << "\n";
	std::cout<<ledger.total_category(account1.id, net.id, entryDirection::Credit, entries, start, end)<< "\n";

}
//LOG 12/01/2026: integer math added, storing values with int64_t presenting in double. 
//NEXT: add fixed precision input support.
