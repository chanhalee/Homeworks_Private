# Define here the models for your scraped items
#
# See documentation in:
# https://docs.scrapy.org/en/latest/topics/items.html

import scrapy

from scrapy.item import Item, Field

class MovieCrawlerItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
	title = scrapy.Field()
	movie_rate = scrapy.Field()
	netizen_rate = scrapy.Field()
	netizen_count = scrapy.Field()
	journalist_score = scrapy.Field()
	journalist_count = scrapy.Field()
	scope = scrapy.Field()
	playing_time = scrapy.Field()
	opening_date = scrapy.Field()
	director = scrapy.Field()
	image = scrapy.Field()
