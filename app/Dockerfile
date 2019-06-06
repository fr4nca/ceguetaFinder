FROM node

WORKDIR /usr/src/app

ARG dbuser=none
ARG dbpw=none
ARG dbhost=none
ARG googleapi=none

ENV DB_USER=$dbuser
ENV DB_PW=$dbpw
ENV DB_HOST=$dbhost
ENV google_api=$googleapi

COPY package*.json ./

RUN yarn install

COPY . .

EXPOSE 80

CMD ["yarn", "start"]