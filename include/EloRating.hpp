#pragma once

#include "Place.hpp"
#include "PlayerName.hpp"

namespace CatanLeaderboardGenerator {

const double EloRatingStandardMaximumUpdateFactor{32.0};

const std::map<uint_least8_t, double> NumberOfPlayersToEloRatingMaximumUpdateFactor{
  {3, EloRatingStandardMaximumUpdateFactor / 2}, // Each player faces off against 2 other players. There are 3 2-player pairs.
  {4, EloRatingStandardMaximumUpdateFactor / 3}, // Each player faces off against 3 other players. There are 6 2-player pairs.
  {5, EloRatingStandardMaximumUpdateFactor / 4}, // Each player faces off against 4 other players. There are 10 2-player pairs.
  {6, EloRatingStandardMaximumUpdateFactor / 5}, // Each player faces off against 5 other players. There are 15 2-player pairs.
  {7, EloRatingStandardMaximumUpdateFactor / 6}, // Each player faces off against 6 other players. There are 21 2-player pairs.
  {8, EloRatingStandardMaximumUpdateFactor / 7}  // Each player faces off against 7 other players. There are 28 2-player pairs.
};

double elo_rating_maximum_update_factor(const uint_least8_t number_of_players) {
  const std::map<uint_least8_t, double>::const_iterator found{NumberOfPlayersToEloRatingMaximumUpdateFactor.find(number_of_players)};
  if (found != NumberOfPlayersToEloRatingMaximumUpdateFactor.cend()) {
    return found->second;
  } else {
    error("A game cannot have " + std::to_string(number_of_players) + " players.");
  }
}

class EloRating {

public:

  constexpr EloRating() noexcept {}

  constexpr EloRating(const double value) noexcept : value_(value) {}

  constexpr double value() const noexcept {
    return value_;
  }

  /// \brief The expected outcome depends on the difference in Elo rating between two opponents.
  double expected_outcome(const EloRating& opponent_elo_rating) const noexcept {
    return 1.0 / (1.0 + std::pow(10.0, (opponent_elo_rating.value() - value_) / 400.0));
  }

  std::string print() const noexcept {
    return std::to_string((int_least64_t)std::round(value_));
  }

  constexpr bool operator==(const EloRating& other) const noexcept {
    return value_ == other.value_;
  }

  constexpr bool operator!=(const EloRating& other) const noexcept {
    return value_ != other.value_;
  }

  constexpr bool operator<(const EloRating& other) const noexcept {
    return value_ < other.value_;
  }

  constexpr bool operator<=(const EloRating& other) const noexcept {
    return value_ <= other.value_;
  }

  constexpr bool operator>(const EloRating& other) const noexcept {
    return value_ > other.value_;
  }

  constexpr bool operator>=(const EloRating& other) const noexcept {
    return value_ >= other.value_;
  }

  constexpr EloRating operator+(const EloRating& other) const noexcept {
    return {value_ + other.value_};
  }

  constexpr EloRating operator+(const double number) const noexcept {
    return {value_ + number};
  }

  constexpr void operator+=(const EloRating& other) noexcept {
    value_ += other.value_;
  }

  constexpr void operator+=(const double number) noexcept {
    value_ += number;
  }

  constexpr EloRating operator-(const EloRating& other) const noexcept {
    return {value_ - other.value_};
  }

  constexpr EloRating operator-(const double number) const noexcept {
    return {value_ - number};
  }

  constexpr void operator-=(const EloRating& other) noexcept {
    value_ -= other.value_;
  }

  constexpr void operator-=(const double number) noexcept {
    value_ -= number;
  }

  constexpr EloRating operator*(const double number) const noexcept {
    return {value_ * number};
  }

  constexpr void operator*=(const double number) noexcept {
    value_ *= number;
  }

  constexpr EloRating operator/(const double number) const noexcept {
    return {value_ / number};
  }

  constexpr void operator/=(const double number) noexcept {
    value_ /= number;
  }

  struct sort {
    bool operator()(const EloRating& elo_rating_1, const EloRating& elo_rating_2) const noexcept {
      return elo_rating_1.value() < elo_rating_2.value();
    }
  };

protected:

  double value_{1000.0};

};

/// \brief Update a player's Elo rating given a set of players and their places and current Elo ratings.
EloRating update_elo_rating(const PlayerName& player_name, const std::map<PlayerName, std::pair<Place, EloRating>, PlayerName::sort>& players_data) noexcept {
  // Maximum update factor.
  const double maximum_update_factor{elo_rating_maximum_update_factor(players_data.size())};
  // Player place and Elo rating.
  std::map<PlayerName, std::pair<Place, EloRating>, PlayerName::sort>::const_iterator found_player{players_data.find(player_name)};
  Place player_place;
  EloRating player_elo_rating;
  if (found_player != players_data.cend()) {
    player_place = found_player->second.first;
    player_elo_rating = found_player->second.second;
  } else {
    error("Player " + player_name.value() + " is not a participant in this game.");
  }
  // Update the Elo rating using the actual and expected outcomes.
  for (const std::pair<PlayerName, std::pair<Place, EloRating>>& current : players_data) {
    if (player_name != current.first) {
      const double actual_outcome{player_place.outcome(current.second.first)};
      const double expected_outcome{player_elo_rating.expected_outcome(current.second.second)};
      player_elo_rating += maximum_update_factor * (actual_outcome - expected_outcome);
    }
  }
  return player_elo_rating;
}

} // namespace CatanLeaderboardGenerator

namespace std {

  template <> struct hash<CatanLeaderboardGenerator::EloRating> {

    size_t operator()(const CatanLeaderboardGenerator::EloRating& elo_rating) const {
      return hash<double>()(elo_rating.value());
    }

  };

} // namespace std