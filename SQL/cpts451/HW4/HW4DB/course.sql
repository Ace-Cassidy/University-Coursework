-- DROP TABLE IF EXISTS Course;

-- CREATE TABLE Course (
--     courseMajor VARCHAR(12),
-- 	courseNo  	VARCHAR(6),
-- 	credits   	INTEGER NOT NULL,
--     enroll_limit INTEGER,
-- 	PRIMARY KEY(courseNo, courseMajor),
-- 	FOREIGN KEY (courseMajor) REFERENCES Majors(major)

-- );

INSERT INTO Course(courseMajor,courseNo,credits,enroll_limit) VALUES('CptS','121',4,24),
																  ('CptS','122',4,25),
																  ('CptS','223',3,25),
																  ('CptS','260',3,3),
																  ('CptS','322',3,20),
																  ('CptS','323',3,19),
																  ('CptS','355',3,22),
																  ('CptS','421',3,15),
																  ('CptS','423',3,15),
																  ('CptS','360',3,22),
																  ('CptS','460',3,22),
																  ('CptS','451',3,10),
																  ('CptS','422',3,15),
																  ('CptS','317',3,20),
																  ('CptS','443',3,10),
																  ('CptS','484',3,25),
																  ('CptS','487',3,35),
																  ('CptS','464',3,5),
																  ('CptS','466',3,2),
																  ('CptS','471',3,3),
																  ('EE','214',3,25),
																  ('EE','221',2,30),
																  ('EE','234',3,15),
																  ('EE','261',3,15),
																  ('EE','262',3,15),
																  ('EE','304',3,10),
																  ('EE','311',3,10),
																  ('EE','321',3,10),
																  ('EE','331',3,12),
																  ('EE','334',3,12),
																  ('EE','341',3,10),
																  ('EE','351',3,2),
																  ('EE','361',3,9),
																  ('EE','362',3,9),
																  ('EE','415',2,15),
																  ('EE','416',3,2),
																  ('EE','431',3,5),
																  ('EE','451',3,5),
																  ('EE','483',3,5),
																  ('EE','476',3,5),
																  ('EE','499',3,2),
																  ('MATH','101',3,30),
																  ('MATH','103',3,25),
																  ('MATH','105',3,15),
																  ('MATH','106',3,10),
																  ('MATH','108',3,10),
																  ('MATH','115',3,10),
																  ('MATH','140',3,10),
																  ('MATH','171',4,10),
																  ('MATH','172',4,10),
																  ('MATH','205',3,10),
																  ('MATH','212',3,10),
																  ('MATH','216',3,10),
																  ('MATH','220',3,10),
																  ('MATH','230',3,10),
																  ('MATH','251',3,4),
																  ('MATH','273',3,10),
																  ('MATH','283',3,10),
																  ('MATH','301',3,10),
																  ('CE','211',3,5),
																  ('CE','215',3,5),
																  ('CE','317',3,5),
																  ('CE','322',3,5),
																  ('CE','330',3,5),
																  ('CE','341',3,5),
																  ('CE','351',3,5),
																  ('CE','401',3,5),
																  ('CE','403',3,5),
																  ('CE','405',3,5),
																  ('CE','414',3,5),
																  ('CE','416',3,5),
																  ('CE','417',3,5),
																  ('ME','116',3,8),
																  ('ME','212',3,8),
																  ('ME','216',3,8),
																  ('ME','220',3,8),
																  ('ME','301',3,8),
																  ('ME','303',3,8),
																  ('ME','305',3,8),
																  ('ME','310',3,8),
																  ('ME','311',3,8),
																  ('ME','313',3,8),
																  ('ME','316',3,8),
																  ('ME','348',3,8),
																  ('ME','401',3,8),
																  ('ME','402',3,8),
																  ('ME','431',3,8),
																  ('ME','439',3,8),
																  ('ME','474',3,8),
																  ('ME','495',3,8),
																  ('ME','499',3,8),
																  ('CHE','110',3,4),
																  ('CHE','211',3,4),
																  ('CHE','321',3,4),
																  ('CHE','334',3,5),
																  ('CHE','398',3,5),
																  ('CHE','433',3,5),
																  ('CHE','451',3,5),
																  ('CHE','476',3,5),
																  ('CHE','495',3,5),
																  ('CHE','498',3,5);

