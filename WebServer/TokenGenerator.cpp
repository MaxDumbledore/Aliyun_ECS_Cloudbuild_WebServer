#include "TokenGenerator.h"
#include <nlohmann/json.hpp>

const std::string SECRET_KEY = "Fly me to the moon.";

std::string TokenGenerator::generate(const std::string& username) {
    return jwt::create<nlohmann_traits>()
        .set_issuer("Aliyun")
        .set_type("JWS")
        .set_payload_claim("username",username)
        .sign(jwt::algorithm::hs256(SECRET_KEY));
}

bool TokenGenerator::verify(const JwtDecoded& decodedToken) {
    auto verifier = jwt::verify<jwt::default_clock, nlohmann_traits>({})
                        .allow_algorithm(jwt::algorithm::hs256{SECRET_KEY})
                        .with_issuer("Aliyun")
                        .with_type("JWS");
    std::error_code ec;
    verifier.verify(decodedToken, ec);
    return !(bool)ec;
}

JwtDecoded TokenGenerator::decode(const std::string& token) {
    return jwt::decode<nlohmann_traits>(token);
}
