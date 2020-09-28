select statecode, name from states;
select * from counties where name like "Prince%";
select states.population_2010 from states inner join senators on states.statecode=senators.statecode where senators.name like "Richard Lugar";
select count(counties.name) from counties inner join states on states.statecode=counties.statecode where states.name like "Maryland";
select name from states where admitted_to_union=(select max(admitted_to_union) from states);
select name from senators where affiliation = "D" and name not in (select distinct(chairman) from committees) order by name;