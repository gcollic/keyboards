FROM debian:stable
MAINTAINER Guillaume Collic <gcollic@gmail.com>
RUN apt-get update && apt-get install -y \
  make \
  gcc-avr \
  avr-libc \
  git
RUN git clone https://github.com/tmk/tmk_core /tmk_core
# Checkout latest commit (at time of writing) by id for deterministic build
RUN cd /tmk_core \
	&& git fetch \
	&& git checkout 221663cab0623ffffbdd761ba6bdd978f9d2bf45
# Add our code
ADD src /src
# Default command : build it
WORKDIR /src
CMD make